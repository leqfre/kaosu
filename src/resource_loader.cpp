#include "resource_loader.hpp"
#include "DxLib.h"

ResourceLoader *ResourceLoader::getInstance()
{
    static ResourceLoader instance;
    return &instance;
}

void ResourceLoader::load()
{
    taikoNoteImages_[0] = LoadGraph("./rsc/don.png");
    taikoNoteImages_[1] = LoadGraph("./rsc/katsu.png");

    taikoHitEffectImages_[0] = LoadGraph("./rsc/taiko-hit300.png");
    taikoHitEffectImages_[1] = LoadGraph("./rsc/taiko-hit100.png");
    taikoHitEffectImages_[2] = LoadGraph("./rsc/taiko-hit0.png");

    judgeCircleImage_ = LoadGraph("./rsc/judge_circle.png");

    taikoHitSounds_[0] = LoadSoundMem("./rsc/taiko-normal-hitfinish.wav");
    taikoHitSounds_[1] = LoadSoundMem("./rsc/taiko-normal-hitclap.wav");
}

std::array<int, taikoNoteTypes> ResourceLoader::getTaikoNoteImages() const
{
    return taikoNoteImages_;
}

std::array<int, taikoNoteTypes> ResourceLoader::getTaikoHitEffectImages() const
{
    return taikoHitEffectImages_;
}

int ResourceLoader::getJudgeCircleImage() const
{
    return judgeCircleImage_;
}

std::array<int, taikoNoteTypes> ResourceLoader::getTaikoHitSounds() const
{
    return taikoHitSounds_;
}