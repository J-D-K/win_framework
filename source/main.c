#include "Window.h"

COLORREF WINDOW_BACKGROUND = RGB(0x4D, 0x4D, 0x4D);
COLORREF EDIT_BACKGROUND = RGB(0x2D, 0x2D, 0x2D);
COLORREF TEXT_COLOR = RGB(0xFF, 0xFF, 0xFF);

void booton(Window *window, WPARAM wParam, void *data)
{
    MessageBox(NULL, "Booton", "WOOP", MB_ICONEXCLAMATION);
}

int WINAPI WinMain(HINSTANCE handle, HINSTANCE pHandle, char *commandline, int showCmd)
{
    Window *mainWindow = windowCreate("MainWindow", "MainWindow", 320, 240, WINDOW_BACKGROUND, handle);
    if (!mainWindow)
    {
        MessageBox(NULL, "Error creating main window!", "Error", MB_ICONERROR);
        return -1;
    }

    // Set the window font to arial.
    windowSetFont(mainWindow, "Franklin Gothic Medium", 16);
    windowSetTextColor(mainWindow, TEXT_COLOR);

    // Add controls/text to window here.
    windowAddText(mainWindow, 0, 0, "~Hello, world~");

    // Show the window.
    windowShow(mainWindow);

    // Update the window.
    while (windowUpdate(mainWindow))
    {
    };
    return 0;
}
