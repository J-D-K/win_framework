#include "Tab.h"
#include <commctrl.h>
#include <string.h>

#include "Window.h"

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

Child *windowAddTabControl(Window *window,
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

    child->handle = CreateWindowEx(0,
                                   WC_TABCONTROL,
                                   NULL,
                                   WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | style,
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

    child->eventFunction = eventFunction;
    child->data = data;

    SendMessage(child->handle, WM_SETFONT, (WPARAM)window->font, 0);

    return child;
}

bool tabControlAddTab(Child *child, int index, char *tabName, Window *window)
{
    TCITEM tab = {.mask = TCIF_TEXT,
                  .dwState = 0,
                  .dwStateMask = 0,
                  .pszText = tabName,
                  .cchTextMax = strlen(tabName),
                  .iImage = -1,
                  .lParam = (LONG_PTR)window};

    return SendMessage(child->handle, TCM_INSERTITEM, index, (LONG_PTR)&tab) != -1;
}
