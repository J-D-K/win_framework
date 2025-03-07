#include "Tab.h"
#include "Child.h"
#include <commctrl.h>
#include <string.h>

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

Child *windowAddTabControl(Window *window, int x, int y, int width, int height, DWORD style)
{
    Child *child = dynamicArrayNew(window->children);
    if (!child)
    {
        return NULL;
    }

    child->handle = CreateWindowEx(0,
                                   WC_TABCONTROLA,
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

    childInitFunctionsDefault(child);

    SendMessage(child->handle, WM_SETFONT, (WPARAM)window->font, 0);

    return child;
}

int tabControlAddTab(Child *child, int index, char *tabTitle)
{
    // Internal tab id.
    static int tabId = 0;

    // This uses the internal index.
    TCITEM tab = {.mask = TCIF_TEXT,
                  .dwState = 0,
                  .dwStateMask = 0,
                  .pszText = tabTitle,
                  .cchTextMax = strlen(tabTitle),
                  .iImage = -1,
                  .lParam = tabId};

    if (SendMessage(child->handle, TCM_INSERTITEM, index, (LONG_PTR)&tab) == -1)
    {
        return -1;
    }
    return tabId++;
}
