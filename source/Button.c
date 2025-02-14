#include "Button.h"

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

bool buttonGetCheck(Window *window)
{
    return SendMessage(window->handle, BM_GETCHECK, 0, 0) == BST_CHECKED;
}

void buttonSetCheck(Window *window, bool check)
{
    SendMessage(window->handle, BM_SETCHECK, check ? BST_CHECKED : BST_UNCHECKED, 0);
}
