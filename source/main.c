#include "Window.h"
#include <unistd.h>

COLORREF WINDOW_BACKGROUND = RGB(0x4D, 0x4D, 0x4D);
COLORREF EDIT_BACKGROUND = RGB(0x2D, 0x2D, 0x2D);
COLORREF TEXT_COLOR = RGB(0xFF, 0xFF, 0xFF);

void getFileName(Window *window, WPARAM wParam, void *data)
{
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

    Window *box = windowAddComboxBox(mainWindow, 8, 8, 240, 160, CBS_DROPDOWN, NULL, NULL);
    comboBoxAddString(box, "String A");
    comboBoxAddString(box, "String B");
    comboBoxAddString(box, "String C");
    comboBoxAddString(box, "String D");

    Window *bar = windowAddProgressBar(mainWindow, 8, 32, 224, 24, PBS_SMOOTH);
    progressBarSetRange(bar, 0, 60);

    // Show the window.
    windowShow(mainWindow);

    // This is just a stupid test loop.
    for (int i = 0; i < 60; i++)
    {
        progressBarAdvance(bar, 1);
        sleep(1);
    }

    // Update the window.
    while (windowUpdate(mainWindow))
    {
    };
    return 0;
}
