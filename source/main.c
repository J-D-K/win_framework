#include "win_framework.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

// Window background color
static const COLORREF WINDOW_BACKGROUND = RGB(0xF0, 0xF0, 0xF0);
// White for tab background.
static const COLORREF TAB_BACKGROUND = RGB(0xFF, 0xFF, 0xFF);
// Text color.
static const COLORREF TEXT_COLOR = RGB(0x00, 0x00, 0x00);

static void buttonFunction(Window *window, WPARAM wParam, LPARAM lParam, void *dataIn)
{
    MessageBox(windowGetHandle(window), "Button clicked.", "WOOOO", MB_ICONEXCLAMATION);
}

static void fileOpenFunction(Window *window, WPARAM wParam, LPARAM lParam, void *dataIn)
{
    MessageBox(windowGetHandle(window),
               "This is where a file dialog would be if I felt like adding it right now.",
               "Laziness kicked in.",
               MB_ICONERROR);
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
                                      1920,
                                      1080,
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

    Child *testButton = windowAddButton(mainWindow, 8, 24, 0, 0, "Click this.", BS_CENTER);
    buttonSetIdealSize(testButton);

    windowAddMenuEvent(mainWindow, openId, fileOpenFunction, NULL);
    childSetCommandFunction(testButton, buttonFunction, NULL);

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
