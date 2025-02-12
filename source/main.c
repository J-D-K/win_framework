#include "Window.h"

COLORREF WINDOW_BACKGROUND = RGB(0x01, 0x22, 0x39);
COLORREF TEXT_COLOR = RGB(0xFF, 0xFF, 0xFF);

void booton(Window *window, WPARAM wParam, void *data)
{
    MessageBox(NULL, "Booton", "WOOP", MB_ICONEXCLAMATION);
}

int WINAPI WinMain(HINSTANCE handle, HINSTANCE pHandle, char *commandline, int showCmd)
{
    Window *mainWindow = windowCreate("MainWindow", "MainWindow", 320, 240, COLOR_BACKGROUND, handle);
    if (!mainWindow)
    {
        MessageBox(NULL, "Error creating main window!", "Error", MB_ICONERROR);
        return -1;
    }

    // Set the window font to arial.
    windowSetFont(mainWindow, "Arial", 14);
    windowSetTextColor(mainWindow, TEXT_COLOR);

    // Add controls/text to window here.
    windowAddText(mainWindow, 0, 0, "~Hello, world~");

    Window *buttonGroup = windowAddButton(mainWindow, 8, 16, 224, 160, NULL, BS_GROUPBOX, NULL, NULL);
    Window *radioA = windowAddButton(mainWindow, 12, 32, 14, 14, "RadioA", BS_AUTORADIOBUTTON | WS_GROUP, NULL, NULL);
    Window *radioB = windowAddButton(mainWindow, 12, 46, 14, 14, "RadioB", BS_AUTORADIOBUTTON, NULL, NULL);
    Window *radioC = windowAddButton(mainWindow, 12, 60, 14, 14, "RadioC", BS_AUTORADIOBUTTON, NULL, NULL);
    Window *button = windowAddButton(mainWindow, 12, 74, 96, AUTO_SIZE, "Button", BS_CENTER, booton, NULL);

    // Show the window.
    windowShow(mainWindow);

    // Update the window.
    while (windowUpdate(mainWindow))
    {
    };
    return 0;
}
