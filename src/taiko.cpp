#include "taiko.hpp"
#include <cstdlib>

namespace
{
    constexpr int judgeLevel = 5;
    constexpr int oneFrameMs = 1000 / 60;

    constexpr int perfectJudgeMs = oneFrameMs * (1 + judgeLevel);
    constexpr int goodJudgeMs    = oneFrameMs * (4 + judgeLevel);
    constexpr int badJudgeMs     = oneFrameMs * 10;
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
    bm_->offset = 400;
    //PlaySoundMem(bm_->music, DX_PLAYTYPE_BACK);

    start_ = std::chrono::system_clock::now();

    position_ = 0;
    targetNoteIndex_ = 0;
}

void Taiko::update()
{
    DrawFormatString(0, 0, GetColor(0, 255, 0), "%d", bm_->hitObjects.size());
    DrawFormatString(0,32, GetColor(0, 255, 0), "%lf", bm_->hitObjects[0][2]);
    DrawFormatString(0,48, GetColor(0, 255, 0), "%d", targetNoteIndex_);

    auto elapsed = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_).count());

    if (bm_->hitObjects[targetNoteIndex_][2] + bm_->offset + (badJudgeMs / 2) < elapsed)
    {
        ++targetNoteIndex_;
    }

    if (gc_->getKey(KEY_INPUT_F) == 1 || gc_->getKey(KEY_INPUT_J) == 1)
    {
        PlaySoundMem(rl_->getTaikoHitSounds()[0], DX_PLAYTYPE_BACK);
        judge(elapsed, Don);
    }

    if (gc_->getKey(KEY_INPUT_D) == 1 || gc_->getKey(KEY_INPUT_K) == 1)
    {
        PlaySoundMem(rl_->getTaikoHitSounds()[1], DX_PLAYTYPE_BACK);
        judge(elapsed, Katsu);
    }

    DrawGraph(100, 300, rl_->getJudgeCircleImage(), TRUE);

    for (int i = 0; i < (signed) bm_->hitObjects.size(); ++i)
    {
        int type = bm_->hitObjects[i][4] == 0 ? rl_->getTaikoNoteImages()[0] : rl_->getTaikoNoteImages()[1];
        DrawGraph((int) (bm_->offset + bm_->hitObjects[i][2] * 1.6 + position_ + 300), 300, type, TRUE);
    }
    position_ -= 26.7;
}

void Taiko::judge(double elapsed, NoteType type)
{
    auto targetNoteType = bm_->hitObjects[targetNoteIndex_][4];

    if (type == Don && targetNoteType != 0)
    {
        return;
    }

    if (type == Katsu && targetNoteType == 0)
    {
        return;
    }

    auto diff = std::abs(bm_->hitObjects[targetNoteIndex_][2] + bm_->offset - elapsed);

    if (diff >= (badJudgeMs / 2))
    {
        return;
    }

    bm_->hitObjects.erase(bm_->hitObjects.begin() + targetNoteIndex_);

    if (diff < (perfectJudgeMs / 2))
    {
        DrawGraph(34, 234, rl_->getTaikoHitEffectImages()[0], TRUE);
    }
    else if (diff < (goodJudgeMs / 2))
    {
        DrawGraph(34, 234, rl_->getTaikoHitEffectImages()[1], TRUE);
    }
    else
    {
        DrawGraph(34, 234, rl_->getTaikoHitEffectImages()[2], TRUE);
    }
}