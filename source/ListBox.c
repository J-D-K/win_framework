#include "ListBox.h"
#include "Child.h"
#include <commctrl.h>

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

Child *windowAddListBox(Window *window, int x, int y, int width, int height, DWORD style)
{
    Child *child = dynamicArrayNew(window->children);
    if (!child)
    {
        return NULL;
    }

    if (height == AUTO_SIZE)
    {
        TEXTMETRIC textMetrics;
        GetTextMetrics(window->context, &textMetrics);

        height = textMetrics.tmHeight + 4;
    }

    child->handle = CreateWindowEx(0,
                                   WC_LISTBOXA,
                                   NULL,
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
        return NULL;
    }

    // Set the font to match the window.
    SendMessage(child->handle, WM_SETFONT, (WPARAM)window->font, 0);

    childInitFunctionsDefault(child);

    return child;
}

bool listBoxAddString(Child *child, const char *string)
{
    return SendMessage(child->handle, LB_ADDSTRING, 0, (LONG_PTR)string) != LB_ERR;
}

int listBoxGetCount(Child *child)
{
    // Technically this can still return LB_ERROR...
    return SendMessage(child->handle, LB_GETCOUNT, 0, 0);
}

int listBoxGetCurrentSelected(Child *child)
{
    return SendMessage(child->handle, LB_GETCURSEL, 0, 0);
}

int listBoxGetSelectedCount(Child *child)
{
    return SendMessage(child->handle, LB_GETSELCOUNT, 0, 0);
}

int listBoxGetSelected(Child *child, int *selectedOut, int maxOut)
{
    return SendMessage(child->handle, LB_GETSELITEMS, maxOut, (LONG_PTR)selectedOut);
}

int listBoxGetTextLength(Child *child, int index)
{
    return SendMessage(child->handle, LB_GETTEXTLEN, index, 0);
}

bool listBoxGetText(Child *child, int index, char *buffer, size_t bufferSize)
{
    if (listBoxGetTextLength(child, index) >= (int)bufferSize)
    {
        // Don't even bother.
        return false;
    }
    return SendMessage(child->handle, LB_GETTEXT, index, (LONG_PTR)buffer) != LB_ERR;
}

bool listBoxDeleteString(Child *child, int index)
{
    if (index < 0 || index >= listBoxGetCount(child))
    {
        return false;
    }
    return SendMessage(child->handle, LB_DELETESTRING, index, 0) == LB_ERR;
}

bool listBoxClear(Child *child)
{
    // Grab the total count in the box
    int count = listBoxGetCount(child);
    if (count <= 0 || count == LB_ERR)
    {
        return false;
    }

    // Loop and nuke
    for (int i = 0; i < count; i++)
    {
        // Always delete 0. The list will collapse as it goes.
        if (!listBoxDeleteString(child, 0))
        {
            return false;
        }
    }
    return true;
}
