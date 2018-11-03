#include "game_controller.hpp"
#include "DxLib.h"

GameController *GameController::getInstance()
{
    static GameController instance;
    return &instance;
}

void GameController::update()
{
    updateKeys();
}

void GameController::updateKeys()
{
    char tmpKey[keysSize];
    GetHitKeyStateAll(tmpKey);

    for (int i = 0; i < keysSize; ++i)
    { 
        if (tmpKey[i] != 0)
        {
            keys[i]++;
        }
        else
        {
            keys[i] = 0;
        }
    }
}

int GameController::getKey(int keyInput)
{
    return keys[keyInput];
}