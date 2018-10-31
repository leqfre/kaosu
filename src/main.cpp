#include "DxLib.h"
#include <memory>
#include <chrono>
#include "resource_loader.hpp"
#include "osu_file_loader.hpp"
#include "taiko.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE);
    SetGraphMode(1600, 900, 32);

    // SetOutApplicationLogValidFlag(FALSE);

    if (DxLib_Init() == 1)
    {
        return -1;
    }

    auto *rl = ResourceLoader::getInstance();
    rl->load();

    auto taiko = std::make_unique<Taiko>();

    PlaySoundFile("./songs/music.mp3", DX_PLAYTYPE_BACK);

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();
        SetDrawScreen(DX_SCREEN_BACK);

        taiko->update();

        ScreenFlip();
    }

    WaitKey();

    DxLib_End();

    return 0;
}