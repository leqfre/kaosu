#include "DxLib.h"
#include <memory>
#include "utils.hpp"
#include "resource_loader.hpp"
#include "game_controller.hpp"
#include "osu_file_loader.hpp"
#include "taiko.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE);
    SetGraphMode(1600, 900, 32);

    // SetOutApplicationLogValidFlag(FALSE);

    if (DxLib_Init() != 0)
    {
        return -1;
    }

    SetDrawMode(DX_DRAWMODE_BILINEAR);

    auto *rl = ResourceLoader::getInstance();
    rl->load();

    auto fileNames = Util::getFileNames("./songs", Util::FOLDERS_ONLY);

    auto taiko = std::make_unique<Taiko>(fileNames[3]);
    taiko->load();

    auto *gc = GameController::getInstance();

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();
        SetDrawScreen(DX_SCREEN_BACK);

        gc->update();
        taiko->update();

        ScreenFlip();
    }

    WaitKey();

    DxLib_End();

    return 0;
}