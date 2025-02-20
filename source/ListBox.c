#include "ListBox.h"
#include <commctrl.h>

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

Child *windowAddListBox(Window *window,
                        int x,
                        int y,
                        int width,
                        int height,
                        DWORD style,
                        EventFunction eventFunction,
                        void *data)
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

    // Save these.
    child->eventFunction = eventFunction;
    child->data = data;

    return child;
}

bool listBoxAddString(Child *child, const char *string)
{
    return SendMessage(child->handle, LB_ADDSTRING, 0, (LONG_PTR)string) != LB_ERR;
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
