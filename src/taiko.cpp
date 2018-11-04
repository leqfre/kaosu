#include "taiko.hpp"
#include <cstdlib>

namespace
{
    constexpr int judgeLevel = 5;
    constexpr int oneFrameMs = 1000 / 60;

    constexpr int perfectJudgeMs = oneFrameMs * (1 + judgeLevel);
    constexpr int goodJudgeMs    = oneFrameMs * (4 + judgeLevel);
    constexpr int badJudgeMs     = oneFrameMs * 10;

    constexpr int noteSize = 128;
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
    bm_->offset = 40;
    PlaySoundMem(bm_->music, DX_PLAYTYPE_BACK);

    start_ = std::chrono::system_clock::now();

    position_ = 0;
    targetNoteIndex_ = 0;
}

void Taiko::update()
{
    DrawFormatString(0, 0, GetColor(0, 255, 0), "%d", bm_->hitObjects.size());
    DrawFormatString(0,32, GetColor(0, 255, 0), "%lf", bm_->hitObjects[0][2]);
    DrawFormatString(0,48, GetColor(0, 255, 0), "%d", targetNoteIndex_);

    drawJudgeCircle();
    auto elapsed = calcElapsed();

    bool isAutoPlay = true;
    bool autoDon = false;
    bool autoKatsu = false;

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

    if (isTargetNoteOutOfJudgeRange(elapsed))
    {
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

    for (int i = 0; i < (signed) bm_->hitObjects.size(); ++i)
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

    if ((inputedNoteType == Don && (targetNoteType != Don && targetNoteType != DonBig))
        || (inputedNoteType == Katsu && (targetNoteType != Katsu && targetNoteType != KatsuBig)))
    {
        drawHitEffect(HE0);
        return;
    }

    if (diff < (perfectJudgeMs / 2))
    {
        drawHitEffect(HE300);
    }
    else if (diff < (goodJudgeMs / 2))
    {
        drawHitEffect(HE100);
    }
    else
    {
        drawHitEffect(HE0);
    }
}

bool Taiko::isTargetNoteOutOfJudgeRange(const double elapsed) const
{
    return (bm_->hitObjects[targetNoteIndex_][2] + bm_->offset + (badJudgeMs / 2) < elapsed);
}

double Taiko::calcElapsed() const
{
    return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_).count());
}

void Taiko::drawJudgeCircle() const
{
    DrawGraph(100, 300, rl_->getJudgeCircleImage(), TRUE);
}

NoteType Taiko::getNoteType(const std::vector<double>& hitObject) const
{
    if (hitObject[4] == 0)
    {
        return Don;
    }

    if (hitObject[4] == 4)
    {
        return DonBig;
    }

    if (hitObject[4] == 8)
    {
        return Katsu;
    }

    if (hitObject[4] == 12)
    {
        return KatsuBig;
    }

    return None;
}

void Taiko::drawNote(const int index, const double elapsed) const
{
    int noteType = getNoteType(bm_->hitObjects[index]);
    double x = (bm_->hitObjects[index][2] - elapsed) / bm_->timingPoints[0][1];
    DrawGraph((int) (bm_->offset + x * noteSize * 4 + 150), 300, rl_->getTaikoNoteImages()[noteType - 1], TRUE);
}

void Taiko::drawHitEffect(const HitEffectType type) const
{
    DrawGraph(34, 234, rl_->getTaikoHitEffectImages()[type], TRUE);
}

void Taiko::playHitSound(const NoteType type) const
{
    auto index = (type == Don || type == DonBig) ? 0 : 1;
    PlaySoundMem(rl_->getTaikoHitSounds()[index], DX_PLAYTYPE_BACK);
}