#include "resource_loader.hpp"
#include "DxLib.h"

ResourceLoader *ResourceLoader::getInstance()
{
    static ResourceLoader instance;
    return &instance;
}

void ResourceLoader::load()
{
    donImage_   = LoadGraph("./rsc/don.png");
    katsuImage_ = LoadGraph("./rsc/katsu.png");
    judgeImage_ = LoadGraph("./rsc/judge_circle.png");
}

int ResourceLoader::getDonImage() const
{
    return donImage_;
}

int ResourceLoader::getKatsuImage() const
{
    return katsuImage_;
}

int ResourceLoader::getJudgeImage() const
{
    return judgeImage_;
}