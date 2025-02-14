#include "ProgressBar.h"
#include <uxtheme.h>

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

Window *windowAddProgressBar(Window *window, int x, int y, int width, int height, DWORD style)
{
    if (!window->children)
    {
        return NULL;
    }

    Window *child = dynamicArrayNew(window->children);
    if (!child)
    {
        return NULL;
    }

    child->handle = CreateWindowEx(0,
                                   PROGRESS_CLASS,
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
    child->appHandle = NULL;
    child->context = NULL;
    child->font = NULL;
    child->children = NULL;
    child->text = NULL;
    child->eventFunction = NULL;
    child->data = NULL;

    return child;
}

void progressBarSetBarColor(Window *window, COLORREF color)
{
    SendMessage(window->handle, PBM_SETBARCOLOR, 0, color);
}

void progressBarSetBackColor(Window *window, COLORREF color)
{
    SendMessage(window->handle, PBM_SETBKCOLOR, 0, color);
}

void progressBarAdvance(Window *window, int advance)
{
    SendMessage(window->handle, PBM_DELTAPOS, advance, 0);
}

void progressBarSetPosition(Window *window, int position)
{
    SendMessage(window->handle, PBM_SETPOS, position, 0);
}

void progressBarSetRange(Window *window, int minimum, int maximum)
{
    SendMessage(window->handle, PBM_SETRANGE32, minimum, maximum);
}
