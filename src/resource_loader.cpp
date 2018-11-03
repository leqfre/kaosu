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
    judgeCircleImage_ = LoadGraph("./rsc/judge_circle.png");

    taikoHitSounds_[0] = LoadSoundMem("./rsc/taiko-normal-hitfinish.wav");
    taikoHitSounds_[1] = LoadSoundMem("./rsc/taiko-normal-hitclap.wav");
}

std::array<int, taikoNoteTypes> ResourceLoader::getTaikoNoteImages() const
{
    return taikoNoteImages_;
}

int ResourceLoader::getJudgeCircleImage() const
{
    return judgeCircleImage_;
}

std::array<int, taikoNoteTypes> ResourceLoader::getTaikoHitSounds() const
{
    return taikoHitSounds_;
}