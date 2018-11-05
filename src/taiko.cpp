#include "taiko.hpp"
#include <cstdlib>

namespace
{
    constexpr int judgeLevel = 5;
    constexpr int oneFrameMs = 1000 / 60;

    constexpr int perfectJudgeMs = oneFrameMs * (1 + judgeLevel);
    constexpr int goodJudgeMs    = oneFrameMs * (4 + judgeLevel);
    constexpr int badJudgeMs     = oneFrameMs * 10;

    constexpr double systemHiSpeed = 0.85;

    constexpr double musicVolume = 0.7;

    constexpr int defaultNoteSize = 128;
    constexpr int noteSmallSize   = 100;
    constexpr int noteBigSize     = 160;

    constexpr int hitEffectSize = 260;

    constexpr int judgeCircleX = 120;
    constexpr int judgeCircleY = 300;

    constexpr int hitEffectX = judgeCircleX - 66;
    constexpr int hitEffectY = judgeCircleY - 66;

    constexpr int maxhitEffectCount_ = 8;
}

Taiko::Taiko(const std::string& song)
    : rl_(ResourceLoader::getInstance()), gc_(GameController::getInstance()), path_("./songs/" + song)
{
}

Taiko::~Taiko()
{
}

void Taiko::load()
{
    bm_ = std::make_unique<OsuFileLoader>(path_)->load();
    bm_->offset = 0;
    ChangeVolumeSoundMem((int) (255 * musicVolume), bm_->music);
    PlaySoundMem(bm_->music, DX_PLAYTYPE_BACK);

    start_ = std::chrono::system_clock::now();

    targetTimingIndex_ = 0;
    targetNoteIndex_ = 0;

    hitEffectCount_ = -1;
    beatmapHiSpeed_ = 0;
    notesInterval_ = 0;
}

void Taiko::update()
{
    DrawFormatString(0,   0, GetColor(0, 255, 0), "%d", bm_->hitObjects.size());
    DrawFormatString(0,  16, GetColor(0, 255, 0), "%lf", (1 - ((double) hitEffectCount_ / maxhitEffectCount_)));
    DrawFormatString(0,  32, GetColor(0, 255, 0), "%lf", bm_->hitObjects[0][2]);
    DrawFormatString(0,  48, GetColor(0, 255, 0), "%lf", bm_->timingPoints[targetTimingIndex_][0]);
    DrawFormatString(0,  64, GetColor(0, 255, 0), "%lf", bm_->timingPoints[targetTimingIndex_][1]);
    DrawFormatString(0,  80, GetColor(0, 255, 0), "%lf", beatmapHiSpeed_);
    DrawFormatString(0,  96, GetColor(0, 255, 0), "%d", targetNoteIndex_);

    drawJudgeCircle();

    auto elapsed = calcElapsed();

    auto isAutoPlay = true;
    auto autoDon = false;
    auto autoKatsu = false;

    if (isAutoPlay && elapsed >= bm_->hitObjects[targetNoteIndex_][2] + bm_->offset)
    {
        auto noteType = getNoteType(bm_->hitObjects[targetNoteIndex_]);

        if (noteType == Don || noteType == DonBig)
        {
            autoDon = true;
        }
        else if (noteType == Katsu || noteType == KatsuBig)
        {
            autoKatsu = true;
        }
    }

    if (isTargetTimingElapsed(elapsed))
    {
        beatmapHiSpeed_ = calcBeatmapHiSpeed();
        ++targetTimingIndex_;
        //++targetTimingIndex_;
    }

    if (isTargetNoteElapsed(elapsed))
    {
        hitEffectCount_ = 0;
        currentHitEffectType_ = HE0;
        ++targetNoteIndex_;
    }

    if (gc_->getKey(KEY_INPUT_F) == 1 || gc_->getKey(KEY_INPUT_J) == 1 || autoDon)
    {
        playHitSound(Don);
        judge(elapsed, Don);
    }

    if (gc_->getKey(KEY_INPUT_D) == 1 || gc_->getKey(KEY_INPUT_K) == 1 || autoKatsu)
    {
        playHitSound(Katsu);
        judge(elapsed, Katsu);
    }

    if (hitEffectCount_ >= 0)
    {
        drawHitEffect(currentHitEffectType_);
    }

    for (int i = bm_->hitObjects.size() - 1; i > -1 ; --i)
    {
        drawNote(i, elapsed);
    }
}

void Taiko::judge(const double elapsed, const NoteType inputedNoteType)
{
    auto diff = std::abs(bm_->hitObjects[targetNoteIndex_][2] + bm_->offset - elapsed);

    if (diff >= (badJudgeMs / 2))
    {
        return;
    }

    auto targetNoteType = getNoteType(bm_->hitObjects[targetNoteIndex_]);
    bm_->hitObjects.erase(bm_->hitObjects.begin() + targetNoteIndex_);

    hitEffectCount_ = 0;

    if ((inputedNoteType == Don && (targetNoteType != Don && targetNoteType != DonBig))
        || (inputedNoteType == Katsu && (targetNoteType != Katsu && targetNoteType != KatsuBig)))
    {
        currentHitEffectType_ = HE0;
        return;
    }

    if (diff < (perfectJudgeMs / 2))
    {
        currentHitEffectType_ = HE300;
    }
    else if (diff < (goodJudgeMs / 2))
    {
        currentHitEffectType_ = HE100;
    }
    else
    {
        currentHitEffectType_ = HE0;
    }
}

bool Taiko::isTargetTimingElapsed(const double elapsed) const
{
    return (bm_->timingPoints[targetTimingIndex_][0] + bm_->offset < elapsed);
}

bool Taiko::isTargetNoteElapsed(const double elapsed) const
{
    return (bm_->hitObjects[targetNoteIndex_][2] + bm_->offset + (badJudgeMs / 2) < elapsed);
}

double Taiko::calcElapsed() const
{
    return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_).count());
}

double Taiko::calcBeatmapHiSpeed()
{
    if (bm_->timingPoints[targetTimingIndex_][6] == 1)
    {
        notesInterval_ = bm_->timingPoints[targetTimingIndex_][1];
        return beatmapHiSpeed_;
    }

    return 100.f / (bm_->timingPoints[targetTimingIndex_][1] * -1.f);

}

void Taiko::drawJudgeCircle() const
{
    DrawGraph(judgeCircleX, judgeCircleY, rl_->getJudgeCircleImage(), TRUE);
}

NoteType Taiko::getNoteType(const std::vector<double>& hitObject) const
{
    switch ((int) hitObject[4])
    {
    case  0:
    case  1: return Don;
    case  2:
    case  3:
    case  8:
    case  9:
    case 10:
    case 11: return Katsu;
    case  4:
    case  5: return DonBig;
    case  6:
    case  7:
    case 12: return KatsuBig;
    default: return None;
    }
}

void Taiko::drawNote(const int index, const double elapsed) const
{
    auto noteType = getNoteType(bm_->hitObjects[index]);
    auto x = (bm_->offset + bm_->hitObjects[index][2] - elapsed) / notesInterval_;
    //auto x = (bm_->offset + bm_->hitObjects[index][2] - elapsed) / bm_->timingPoints[0][1];

    auto isBigNote = (noteType == DonBig) || (noteType == KatsuBig);
    auto noteSize = isBigNote ? noteBigSize : noteSmallSize;
    auto centerAdjust = (defaultNoteSize - noteSize) / 2;

    auto x1 = (int) (x * noteSmallSize * 4 * systemHiSpeed * beatmapHiSpeed_ + 130) + centerAdjust;
    auto x2 = x1 + noteSize;
    auto y1 = judgeCircleY + centerAdjust;
    auto y2 = y1 + noteSize;

    DrawExtendGraph(x1, y1, x2, y2, rl_->getTaikoNoteImages()[noteType - 1], TRUE);
}

void Taiko::drawHitEffect(const HitEffectType type)
{
    auto diff = pow(hitEffectCount_ - (maxhitEffectCount_ / 2), 2);

    auto x1 = (int) (hitEffectX + diff);
    auto x2 = (int) (hitEffectX + hitEffectSize - diff);
    auto y1 = (int) (hitEffectY + diff);
    auto y2 = (int) (hitEffectY + hitEffectSize - diff);

    auto transparent = (int) (255 * (1 - ((double) hitEffectCount_ / maxhitEffectCount_)));

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, transparent);
    DrawExtendGraph(x1, y1, x2, y2, rl_->getTaikoHitEffectImages()[type], TRUE) ;
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    if (++hitEffectCount_ > maxhitEffectCount_)
    {
        hitEffectCount_ = -1;
    }
}

void Taiko::playHitSound(const NoteType type) const
{
    auto index = (type == Don || type == DonBig) ? 0 : 1;
    PlaySoundMem(rl_->getTaikoHitSounds()[index], DX_PLAYTYPE_BACK);
}