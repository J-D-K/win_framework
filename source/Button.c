#include "Button.h"
#include "Child.h"
#include <commctrl.h>

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

Child *windowAddButton(Window *window, int x, int y, int width, int height, const char *buttonText, DWORD style)
{
    // Children check
    if (!window->children)
    {
        return NULL;
    }

    Child *child = dynamicArrayNew(window->children);
    if (!child)
    {
        return NULL;
    }

    // Need this to calculate button height.
    if (height == AUTO_SIZE)
    {
        TEXTMETRIC textMetrics;
        GetTextMetrics(window->context, &textMetrics);

        height = textMetrics.tmHeight + 4;
    }

    // Create the button.
    child->handle = CreateWindowEx(0,
                                   WC_BUTTONA,
                                   buttonText,
                                   WS_CHILD | WS_VISIBLE | style,
                                   x,
                                   y,
                                   width,
                                   height,
                                   window->handle,
                                   NULL,
                                   window->appHandle,
                                   NULL);
    if (!child->handle)
    {
        // Child will be freed when the window is destroyed regardless.
        return NULL;
    }

    childInitFunctionsDefault(child);

    SendMessage(child->handle, WM_SETFONT, (WPARAM)window->font, 0);

    return child;
}

int buttonGetWidth(Child *child)
{
    // Rect to get coords.
    RECT buttonRect = {0};

    if (GetWindowRect(child->handle, &buttonRect) == FALSE)
    {
        return -1;
    }
    return buttonRect.right - buttonRect.left;
}

int buttonGetHeight(Child *child)
{
    RECT buttonRect = {0};

    if (GetWindowRect(child->handle, &buttonRect) == FALSE)
    {
        return -1;
    }
    return buttonRect.bottom - buttonRect.top;
}

bool buttonSetIdealSize(Child *child)
{
    // Struct to get the size.
    SIZE buttonSize = {0};
    if (SendMessage(child->handle, BCM_GETIDEALSIZE, 0, (LONG_PTR)&buttonSize) == FALSE)
    {
        return false;
    }
    // I guess this is the way to do this?
    return SetWindowPos(child->handle, NULL, 0, 0, buttonSize.cx, buttonSize.cy, SWP_NOMOVE) == TRUE;
}

bool buttonGetCheck(Child *child)
{
    return SendMessage(child->handle, BM_GETCHECK, 0, 0) == BST_CHECKED;
}

void buttonSetCheck(Child *child, bool check)
{
    SendMessage(child->handle, BM_SETCHECK, check ? BST_CHECKED : BST_UNCHECKED, 0);
}
