#include "Window.h"
#include <stdio.h>
#include <string.h>

// Window background color
static const COLORREF WINDOW_BACKGROUND = RGB(0xBB, 0xBB, 0xBB);
// Text color.
static const COLORREF TEXT_COLOR = RGB(0x00, 0x00, 0x00);

// Declarations here. Def's later.
static void fileOpenClick(Window *window, WPARAM wParam, void *data)
{
    MessageBox(windowGetHandle(window), "Open Clicked!", "WEW", MB_ICONEXCLAMATION);
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
                                      640,
                                      480,
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

    // Setup menu stuff.
    // Only gonna bother with one cause if it works, the rest should too.
    windowAddMenuEvent(mainWindow, openId, fileOpenClick, NULL);

    // Set text crap.
    windowSetFont(mainWindow, "Arial", 14);
    windowSetTextColor(mainWindow, TEXT_COLOR);

    int y = 0;
    Child *edit = windowAddEdit(mainWindow, 0, y, 224, AUTO_SIZE, ES_AUTOHSCROLL, NULL, NULL);

    windowShow(mainWindow);

    while (windowUpdate(mainWindow))
    {
    }
    // Window automatically destroys itself when the WM_CLOSE command is received.
    return 0;
}
