#include "win_framework.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

// Window background color
static const COLORREF WINDOW_BACKGROUND = RGB(0x2D, 0x2D, 0x2D);
// Text color.
static const COLORREF TEXT_COLOR = RGB(0xFF, 0xFF, 0xFF);

static void buttonClick(Window *window, WPARAM wParam, LPARAM lParam, void *dataIn)
{
    MessageBox(windowGetHandle(window), "LOL", "Button", MB_ICONASTERISK);
}

int WINAPI WinMain(HINSTANCE appHandle, HINSTANCE pHInstance, char *commandline, int cmdShow)
{
    // Creating menus goes reverse compared to how you'd think.
    // File menu
    Menu *fileMenu = menuCreate();
    int openId = menuAddMenu(fileMenu, "Open", NULL);

    // Main menu.
    Menu *mainMenu = menuCreate();
    menuAddMenu(mainMenu, "File", fileMenu);
    menuAddMenu(mainMenu, "Edit", NULL);
    menuAddMenu(mainMenu, "Settings", NULL);
    menuAddMenu(mainMenu, "About", NULL);

    Window *mainWindow = windowCreate("win_framework",
                                      "Main Window",
                                      WINDOW_CENTER,
                                      WINDOW_CENTER,
                                      1280,
                                      720,
                                      WINDOW_MAIN_DEFAULT_STYLE,
                                      WINDOW_BACKGROUND,
                                      mainMenu,
                                      NULL,
                                      appHandle);
    if (!mainWindow)
    {
        MessageBox(NULL, "Error creating main window.", "Error", MB_ICONERROR);
        return -1;
    }

    // Set text crap.
    windowSetFont(mainWindow, "Arial", 14);
    windowSetTextColor(mainWindow, TEXT_COLOR);

    // Set button shit.
    buttonSetBorderColor(RGB(0x20, 0x20, 0x20));
    buttonSetBackgroundColor(RGB(0x3D, 0x3D, 0x3D));
    buttonSetTextColor(RGB(0xFF, 0xFF, 0xFF));

    // Set edit shit.
    editSetBorderColor(RGB(0x20, 0x20, 0x20));
    editSetBackgroundColor(RGB(0x1D, 0x1D, 0x1D));
    editSetTextColor(RGB(0xFF, 0xFF, 0xFF));

    Child *input = windowAddEdit(mainWindow, 16, 16, 480, AUTO_SIZE, ES_AUTOHSCROLL);

    Child *button = windowAddButton(mainWindow, 16, 48, 0, 0, "Test Button", BS_CENTER);
    buttonSetIdealSize(button);

    childSetCommandFunction(button, buttonClick, NULL);

    windowShow(mainWindow);

    while (windowUpdate(mainWindow))
    {
    }
    // Window automatically destroys itself when the WM_CLOSE command is received.
    // But these don't? Not sure.
    menuDestroy(fileMenu);
    menuDestroy(mainMenu);
    return 0;
}
