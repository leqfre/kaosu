#include "DxLib.h"
#include <memory>
#include "osu_file_loader.hpp"

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
    auto donH   = LoadGraph( "./rsc/don.png" );
    auto katsuH = LoadGraph( "./rsc/katsu.png" );

    auto ofl = std::make_unique<OsuFileLoader>("./songs/notes.osu");
    ofl->load();

    int x = 0;

    auto hitObjects = ofl->getHitObjects();

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();
        SetDrawScreen(DX_SCREEN_BACK);

        for (const auto &hitObject : hitObjects)
        {
            DrawGraph((int) (hitObject[2] * 1.6 + x), 300, hitObject[4] == 0 ? donH : katsuH, TRUE);
        }
        x -= 26;

        ScreenFlip();
    }

    WaitKey();

    DxLib_End();

    return 0;
}