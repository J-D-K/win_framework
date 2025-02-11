#include "Window.h"

COLORREF WINDOW_BACKGROUND = RGB(0x4D, 0x4D, 0x4D);
COLORREF TEXT_COLOR = RGB(0xFF, 0xFF, 0xFF);

int WINAPI WinMain(HINSTANCE handle, HINSTANCE pHandle, char *commandline, int showCmd)
{
    Window *mainWindow =
        windowCreate("MainWindow", "MainWindow", 320, 240, "Arial", WINDOW_BACKGROUND, TEXT_COLOR, handle);
    if (!mainWindow)
    {
        MessageBox(NULL, "Error creating main window!", "Error", MB_ICONERROR);
        return -1;
    }

    // Add controls/text to window here.
    windowAddText(mainWindow, 0, 0, "~Hello, world~");

    // Show the window.
    windowShow(mainWindow);

    // Update the window.
    while (windowUpdate(mainWindow))
    {
    };
    // Window frees itself when WM_QUIT is sent.
    return 0;
}
