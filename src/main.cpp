#include "DxLib.h"
#include <memory>
#include <chrono>
#include "osu_file_loader.hpp"

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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE);
    SetGraphMode(1600, 900, 32);

    // SetOutApplicationLogValidFlag(FALSE);

    if (DxLib_Init() == 1)
    {
        return -1;
    }

    // LOAD RESOURCES
    auto donH   = LoadGraph("./rsc/don.png");
    auto katsuH = LoadGraph("./rsc/katsu.png");
    auto judgeH = LoadGraph("./rsc/judge_circle.png");

    auto ofl = std::make_unique<OsuFileLoader>("./songs/notes.osu");
    ofl->load();

    double speed = 0;
    double offset = 400;

    auto hitObjects = ofl->getHitObjects();

    std::chrono::system_clock::time_point  start;
    start = std::chrono::system_clock::now();
    PlaySoundFile("./songs/music.mp3", DX_PLAYTYPE_BACK);

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();
        SetDrawScreen(DX_SCREEN_BACK);
        updateKey();

        if (key[KEY_INPUT_F] == 1 || key[KEY_INPUT_J] == 1)
        {
            hitObjects.erase(hitObjects.begin());
        }

        if (key[KEY_INPUT_D] == 1 || key[KEY_INPUT_K] == 1)
        {
            hitObjects.erase(hitObjects.begin());
        }

        DrawGraph(200, 300, judgeH, TRUE);

        for (int i = 0; i < 10; ++i)
        {
            DrawGraph((int) (offset + hitObjects[i][2] * 1.6 + speed), 300, hitObjects[i][4] == 0 ? donH : katsuH, TRUE);
        }
        speed -= 26.7;

        ScreenFlip();
    }

    WaitKey();

    DxLib_End();

    return 0;
}