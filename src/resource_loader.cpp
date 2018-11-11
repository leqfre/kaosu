#include "resource_loader.hpp"
#include "DxLib.h"
#include "common_const.hpp"

ResourceLoader *ResourceLoader::getInstance()
{
    static ResourceLoader instance;
    return &instance;
}

void ResourceLoader::load()
{
    font_ = CreateFontToHandle("./rsc/PhantasmAllCaps.ttf", COMBO_FONT_SIZE, COMBO_FONT_THICK, DX_FONTTYPE_ANTIALIASING);

    taikoNoteImages_[0] = LoadGraph("./rsc/don.png");
    taikoNoteImages_[1] = LoadGraph("./rsc/katsu.png");
    taikoNoteImages_[2] = LoadGraph("./rsc/don_big.png");
    taikoNoteImages_[3] = LoadGraph("./rsc/katsu_big.png");

    taikoHitEffectImages_[0] = LoadGraph("./rsc/taiko-hit300.png");
    taikoHitEffectImages_[1] = LoadGraph("./rsc/taiko-hit300k.png");
    taikoHitEffectImages_[2] = LoadGraph("./rsc/taiko-hit100.png");
    taikoHitEffectImages_[3] = LoadGraph("./rsc/taiko-hit100k.png");
    taikoHitEffectImages_[4] = LoadGraph("./rsc/taiko-hit0.png");

    judgeCircleImage_ = LoadGraph("./rsc/judge_circle.png");

    taikoHitSounds_[0] = LoadSoundMem("./rsc/taiko-normal-hitfinish.wav");
    taikoHitSounds_[1] = LoadSoundMem("./rsc/taiko-normal-hitclap.wav");
}

int ResourceLoader::getFont() const
{
    return font_;
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