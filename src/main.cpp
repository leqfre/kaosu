#include "DxLib.h"

enum NoteType
{
    Circle,
    Slider,
    Spinner
};

enum NoteEffect
{
    None,
    Whistle,
    Finish,
    Clap
};

typedef struct
{
    int x;
    int y;
    int vx;
    int vy;

    NoteType type;
    NoteEffect effect;
} Note;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE);
    SetGraphMode(640, 480, 32);

    // SetOutApplicationLogValidFlag(FALSE);

    if (DxLib_Init() == 1)
    {
        return -1;
    }

    // LOAD RESOURCES
    int handle = LoadGraph( "./rsc/taiko.png" );

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