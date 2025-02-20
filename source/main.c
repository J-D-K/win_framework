#include "Window.h"
#include <stdio.h>
#include <string.h>

// Window background color
static const COLORREF WINDOW_BACKGROUND = RGB(0xBB, 0xBB, 0xBB);
// Text color.
static const COLORREF TEXT_COLOR = RGB(0x00, 0x00, 0x00);

void buttonClick(Window *window, WPARAM wParam, void *dataIn)
{
    // This should get a pointer to the input.
    Child *textInput = (Child *)dataIn;

    // Get the length of the text.
    int inputLength = editGetTextLength(textInput);
    if (inputLength <= 0)
    {
        // Just bail. There's nothing to show.
        return;
    }

    // Get the length. Add one, because I don't think Windows includes the null terminator.
    char inputBuffer[inputLength + 1];
    memset(inputBuffer, 0x00, inputLength + 1);
    editGetText(textInput, inputBuffer, inputLength + 1);

    MessageBox(windowGetHandle(window), inputBuffer, "Input Text", MB_ICONEXCLAMATION);
}

int WINAPI WinMain(HINSTANCE appHandle, HINSTANCE pHInstance, char *commandline, int cmdShow)
{
    Window *mainWindow = windowCreate("win_framework",
                                      "Main Window",
                                      640,
                                      480,
                                      WINDOW_MAIN_DEFAULT_STYLE,
                                      WINDOW_BACKGROUND,
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

    Child *listBox = windowAddListBox(mainWindow, 0, 0, 160, 240, LBS_NOTIFY | WS_VSCROLL, NULL, NULL);

    for (size_t i = 0; i < 48; i++)
    {
        char nameBuffer[32];
        snprintf(nameBuffer, 32, "%u", i);
        listBoxAddString(listBox, nameBuffer);
    }

    windowShow(mainWindow);

    while (windowUpdate(mainWindow))
    {
    }
    // Window automatically destroys itself when the WM_CLOSE command is received.
    return 0;
}
