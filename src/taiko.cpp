#include "taiko.hpp"

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
    bm_->offset = 200;
    PlaySoundMem(bm_->music, DX_PLAYTYPE_BACK);
}

void Taiko::update()
{
    //std::chrono::system_clock::time_point  start;
    //start = std::chrono::system_clock::now();
    updateKey();

    if (key[KEY_INPUT_F] == 1 || key[KEY_INPUT_J] == 1)
    {
        bm_->hitObjects.erase(bm_->hitObjects.begin());
        PlaySoundMem(rl_->getTaikoHitSounds()[0], DX_PLAYTYPE_BACK);
    }

    if (key[KEY_INPUT_D] == 1 || key[KEY_INPUT_K] == 1)
    {
        bm_->hitObjects.erase(bm_->hitObjects.begin());
        PlaySoundMem(rl_->getTaikoHitSounds()[1], DX_PLAYTYPE_BACK);
    }

    DrawGraph(200, 300, rl_->getJudgeCircleImage(), TRUE);

    for (int i = 0; i < (signed) bm_->hitObjects.size(); ++i)
    {
        DrawGraph((int) (bm_->offset + bm_->hitObjects[i][2] * 1.6 + position_), 300, bm_->hitObjects[i][4] == 0 ? rl_->getTaikoNoteImages()[0] : rl_->getTaikoNoteImages()[1], TRUE);
    }
    position_ -= 26.7;
}