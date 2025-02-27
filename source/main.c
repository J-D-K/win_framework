#include "Window.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

// Window background color
static const COLORREF WINDOW_BACKGROUND = RGB(0xBB, 0xBB, 0xBB);
// Text color.
static const COLORREF TEXT_COLOR = RGB(0x00, 0x00, 0x00);

// Declarations here. Def's later.
static void fileOpenClick(Window *window, WPARAM wParam, LPARAM lParam, void *data)
{
    // Buffer for file name.
    char fileBuffer[256] = {0};

    // Cast the target edit.
    Child *edit = (Child *)data;

    if (!getOpenFilename(window, "PICK A FILE", "Text\0*.txt\0C Files\0*.c\0C++ files\0*.cpp\0\0", 0, fileBuffer, 256))
    {
        return;
    }

    // Try to open the file.
    FILE *target = fopen(fileBuffer, "r");
    if (!target)
    {
        MessageBox(windowGetHandle(window), "Error opening file for reading!", "Error", MB_ICONERROR);
        return;
    }

    // Get the file's size
    fseek(target, 0, SEEK_END);
    size_t fileSize = ftell(target);
    fseek(target, 0, SEEK_SET);

    // Allocate buffer for reading. Heap so we know for sure we have enough space.
    char *fileContents = malloc(fileSize);
    if (!fileContents)
    {
        MessageBox(windowGetHandle(window), "Error allocating buffer to read file!", "ERROROROR", MB_ICONERROR);
        goto _ABORT_MISSION_;
    }

    fread(fileContents, 1, fileSize, target);

    // Set the edit's text to what I read from the file. I'd say we, but we both know you didn't do anything.
    editSetText(edit, fileContents);

_ABORT_MISSION_:
    if (fileContents)
    {
        free(fileContents);
    }

    if (target)
    {
        fclose(target);
    }
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
    // Set text crap.
    windowSetFont(mainWindow, "Arial", 14);
    windowSetTextColor(mainWindow, TEXT_COLOR);

    // Add a multiline edit that takes up the entire window to view whatever I open.
    Child *edit =
        windowAddEdit(mainWindow, 0, 0, 634, 430, WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_WANTRETURN, NULL, NULL);

    // Setup menu stuff.
    // Only gonna bother with one cause if it works, the rest should too.
    windowAddMenuEvent(mainWindow, openId, fileOpenClick, edit);

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
