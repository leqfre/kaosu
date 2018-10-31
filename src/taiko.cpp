#include "taiko.hpp"

Taiko::Taiko()
    : rl_(ResourceLoader::getInstance())
{
    ofl_ = std::make_unique<OsuFileLoader>("./songs/notes.osu");
    ofl_->load();

    hitObjects_ = ofl_->getHitObjects();

    bpm_ = 26.7;
    offset_ = 400;
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


void Taiko::update()
{
    //std::chrono::system_clock::time_point  start;
    //start = std::chrono::system_clock::now();
    updateKey();

    if (key[KEY_INPUT_F] == 1 || key[KEY_INPUT_J] == 1)
    {
        hitObjects_.erase(hitObjects_.begin());
    }

    if (key[KEY_INPUT_D] == 1 || key[KEY_INPUT_K] == 1)
    {
        hitObjects_.erase(hitObjects_.begin());
    }

    DrawGraph(200, 300, rl_->getJudgeImage(), TRUE);

    for (int i = 0; i < 10; ++i)
    {
        DrawGraph((int) (offset_ + hitObjects_[i][2] * 1.6 + position_), 300, hitObjects_[i][4] == 0 ? rl_->getDonImage() : rl_->getKatsuImage(), TRUE);
    }
    position_ -= 26.7;
}