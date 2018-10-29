#include "DxLib.h"
#include <memory>
#include "osu_file_loader.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE);
    SetGraphMode(1024, 600, 32);

    // SetOutApplicationLogValidFlag(FALSE);

    if (DxLib_Init() == 1)
    {
        return -1;
    }

    // LOAD RESOURCES
    auto handle = LoadGraph( "./rsc/taiko.png" );
    auto ofl    = std::make_unique<OsuFileLoader>("./songs/notes.osu");
    ofl->load();

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();
        SetDrawScreen(DX_SCREEN_BACK);

        DrawGraph(0, 0, handle, TRUE) ;

        ScreenFlip();
    }

    WaitKey();

    DxLib_End();

    return 0;
}