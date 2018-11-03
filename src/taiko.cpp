#include "taiko.hpp"
#include <cstdlib>

namespace
{
    constexpr int judgeLevel = 5;
    constexpr int oneFrameMs = 1000 / 60;

    constexpr int perfectJudgeMs = oneFrameMs * (1 + judgeLevel);
    constexpr int goodJudgeMs    = oneFrameMs * (2 + judgeLevel);
    constexpr int badJudgeMs     = oneFrameMs * (3 + judgeLevel);
}

Taiko::Taiko(const std::string& song)
    : rl_(ResourceLoader::getInstance())
{
    process_ = Load;
    path_ = "./songs/" + song;
}

Taiko::~Taiko()
{
}

int key[256];

int updateKey()
{
    char tmpKey[256];
    GetHitKeyStateAll(tmpKey);

    for (int i = 0; i < 256; ++i)
    { 
        if (tmpKey[i] != 0)
        {
            key[i]++;
        }
        else
        {
            key[i] = 0;
        }
    }
    return 0;
}

void Taiko::load()
{
    bm_ = std::make_unique<OsuFileLoader>(path_)->load();
    bm_->offset = 130;
    PlaySoundMem(bm_->music, DX_PLAYTYPE_BACK);

    start_ = std::chrono::system_clock::now();

    position_ = 0;
    targetNote_ = 0;
}

void Taiko::update()
{
    updateKey();

    auto ms = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start_).count() / 1000.0;
    auto elapsed_ = static_cast<double>(ms);

    DrawFormatString(0, 0, GetColor(0, 255, 0), "%d", bm_->hitObjects.size());
    DrawFormatString(0,16, GetColor(0, 255, 0), "%lf", elapsed_);
    DrawFormatString(0,32, GetColor(0, 255, 0), "%lf", bm_->hitObjects[0][2]);
    DrawFormatString(0,48, GetColor(0, 255, 0), "%d", targetNote_);


    if (bm_->hitObjects[targetNote_][2] + (badJudgeMs / 2) < elapsed_)
    {
        //bm_->hitObjects.erase(bm_->hitObjects.begin());
        ++targetNote_;
    }

    if (key[KEY_INPUT_F] == 1 || key[KEY_INPUT_J] == 1)
    {
        PlaySoundMem(rl_->getTaikoHitSounds()[0], DX_PLAYTYPE_BACK);

        if (std::abs(bm_->hitObjects[targetNote_][2] - elapsed_) < (perfectJudgeMs / 2))
        {
            bm_->hitObjects.erase(bm_->hitObjects.begin() + targetNote_);
            DrawGraph(34, 234, rl_->getTaikoHitEffectImages()[0], TRUE);
        }
        else if (std::abs(bm_->hitObjects[targetNote_][2] - elapsed_) < (goodJudgeMs / 2))
        {
            bm_->hitObjects.erase(bm_->hitObjects.begin() + targetNote_);
            DrawGraph(34, 234, rl_->getTaikoHitEffectImages()[1], TRUE);
        }
    }

    if (key[KEY_INPUT_D] == 1 || key[KEY_INPUT_K] == 1)
    {
        PlaySoundMem(rl_->getTaikoHitSounds()[1], DX_PLAYTYPE_BACK);

        if (std::abs(bm_->hitObjects[targetNote_][2] - elapsed_) < (perfectJudgeMs / 2))
        {
            bm_->hitObjects.erase(bm_->hitObjects.begin() + targetNote_);
            DrawGraph(34, 234, rl_->getTaikoHitEffectImages()[0], TRUE);
        }
        else if (std::abs(bm_->hitObjects[targetNote_][2] - elapsed_) < (goodJudgeMs / 2))
        {
            bm_->hitObjects.erase(bm_->hitObjects.begin() + targetNote_);
            DrawGraph(34, 234, rl_->getTaikoHitEffectImages()[1], TRUE);
        }
    }

    DrawGraph(100, 300, rl_->getJudgeCircleImage(), TRUE);

    for (int i = 0; i < (signed) bm_->hitObjects.size(); ++i)
    {
        DrawGraph((int) (bm_->offset + bm_->hitObjects[i][2] * 1.6 + position_), 300, bm_->hitObjects[i][4] == 0 ? rl_->getTaikoNoteImages()[0] : rl_->getTaikoNoteImages()[1], TRUE);
    }
    position_ -= 26.7;
}