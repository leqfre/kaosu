#include "taiko.hpp"
#include <cstdlib>
#include "taiko_common.hpp"

namespace
{
    constexpr int judgeLevel = 5;
    constexpr int oneFrameMs = 1000 / 60;

    constexpr int perfectJudgeMs = oneFrameMs * (1 + judgeLevel);
    constexpr int goodJudgeMs    = oneFrameMs * (4 + judgeLevel);
    constexpr int badJudgeMs     = oneFrameMs * 10;

    constexpr double musicVolume = 0.7;

    constexpr int hitEffectSize = 260;

    constexpr int judgeCircleX = 120;
    constexpr int judgeCircleY = 300;

    constexpr int hitEffectX = judgeCircleX - 66;
    constexpr int hitEffectY = judgeCircleY - 66;

    constexpr int maxhitEffectCount_ = 8;

    constexpr int maxDisplayNotes = 50;
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
    ChangeVolumeSoundMem((int) (255 * musicVolume), bm_->music);
    
    auto tp = std::make_unique<TaikoParser>();
    notes_ = tp->parse(bm_->hitObjects, bm_->timingPoints);

    PlaySoundMem(bm_->music, DX_PLAYTYPE_BACK);
    start_ = std::chrono::system_clock::now();

    targetNoteIndex_ = 0;
    hitEffectCount_ = -1;
}

void Taiko::update()
{
    DrawFormatString(0,   0, GetColor(0, 255, 0), "%d", notes_.size());
    DrawFormatString(0,  16, GetColor(0, 255, 0), "%lf", notes_[targetNoteIndex_]->vx);
    DrawFormatString(0,  32, GetColor(0, 255, 0), "%lf", notes_[targetNoteIndex_]->timing);
    DrawFormatString(0,  48, GetColor(0, 255, 0), "%d", targetNoteIndex_);

    drawJudgeCircle();

    auto elapsed = calcElapsed();

    auto isAutoPlay = true;
    auto autoDon = false;
    auto autoKatsu = false;

    if (isAutoPlay && elapsed >= notes_[targetNoteIndex_]->timing)
    {
        auto noteType = notes_[targetNoteIndex_]->type;

        if (noteType == Don || noteType == DonBig)
        {
            autoDon = true;
        }
        else if (noteType == Katsu || noteType == KatsuBig)
        {
            autoKatsu = true;
        }
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

    auto initialValue = maxDisplayNotes > notes_.size() - 1 ? notes_.size() - 1 : maxDisplayNotes;

    for (int i = initialValue; i > -1 ; --i)
    {
        drawNote(i, elapsed);
    }
}

void Taiko::judge(const double elapsed, const NoteType inputedNoteType)
{
    auto diff = std::abs(notes_[targetNoteIndex_]->timing - elapsed);

    if (diff >= (badJudgeMs / 2))
    {
        return;
    }

    auto targetNoteType = notes_[targetNoteIndex_]->type;
    notes_.erase(notes_.begin() + targetNoteIndex_);

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

bool Taiko::isTargetNoteElapsed(const double elapsed) const
{
    return (notes_[targetNoteIndex_]->timing + (badJudgeMs / 2) < elapsed);
}

double Taiko::calcElapsed() const
{
    return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_).count());
}

void Taiko::drawJudgeCircle() const
{
    DrawGraph(judgeCircleX, judgeCircleY, rl_->getJudgeCircleImage(), TRUE);
}

void Taiko::drawNote(const int index, const double elapsed) const
{
    auto time = notes_[index]->timing - elapsed;

    auto isBig = (notes_[index]->type == DonBig || notes_[index]->type == KatsuBig);
    auto noteSize = isBig ? noteBigSize : noteSmallSize;

    auto adjust = (defaultNoteSize - noteSize) / 2;

    auto x1 = (int) (time * notes_[index]->vx + 130) + adjust;
    auto x2 = x1 + noteSize;
    auto y1 = judgeCircleY + adjust;
    auto y2 = y1 + noteSize;

    DrawExtendGraph(x1, y1, x2, y2, rl_->getTaikoNoteImages()[notes_[index]->type - 1], TRUE);
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