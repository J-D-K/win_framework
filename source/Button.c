#include "Button.h"

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

Child *windowAddButton(Window *window,
                       int x,
                       int y,
                       int width,
                       int height,
                       const char *buttonText,
                       DWORD style,
                       EventFunction eventFunction,
                       void *data)
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
                                   "BUTTON",
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

    // Make sure we set the function and ID.
    child->eventFunction = eventFunction;
    child->data = data;

    SendMessage(child->handle, WM_SETFONT, (WPARAM)window->font, MAKELPARAM(FALSE, 0));

    return child;
}

bool buttonGetCheck(Child *child)
{
    return SendMessage(child->handle, BM_GETCHECK, 0, 0) == BST_CHECKED;
}

void buttonSetCheck(Child *child, bool check)
{
    SendMessage(child->handle, BM_SETCHECK, check ? BST_CHECKED : BST_UNCHECKED, 0);
}
