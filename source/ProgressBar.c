#include "ProgressBar.h"
#include <commctrl.h>

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

Child *windowAddProgressBar(Window *window, int x, int y, int width, int height, DWORD style)
{
    if (!window->children)
    {
        return NULL;
    }

    Child *child = dynamicArrayNew(window->children);
    if (!child)
    {
        return NULL;
    }

    child->handle = CreateWindowEx(0,
                                   PROGRESS_CLASSA,
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

    // NULL the rest for this.
    child->eventFunction = NULL;
    child->data = NULL;

    return child;
}

void progressBarSetBarColor(Child *child, COLORREF color)
{
    SendMessage(child->handle, PBM_SETBARCOLOR, 0, color);
}

void progressBarSetBackColor(Child *child, COLORREF color)
{
    SendMessage(child->handle, PBM_SETBKCOLOR, 0, color);
}

void progressBarAdvance(Child *child, int advance)
{
    SendMessage(child->handle, PBM_DELTAPOS, advance, 0);
}

void progressBarSetPosition(Child *child, int position)
{
    SendMessage(child->handle, PBM_SETPOS, position, 0);
}

void progressBarSetRange(Child *child, int minimum, int maximum)
{
    SendMessage(child->handle, PBM_SETRANGE32, minimum, maximum);
}
