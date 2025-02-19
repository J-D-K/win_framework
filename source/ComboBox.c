#include "ComboBox.h"

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

Child *windowAddComboxBox(Window *window,
                          int x,
                          int y,
                          int width,
                          int height,
                          DWORD style,
                          EventFunction eventFunction,
                          void *data)
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
                                   "COMBOBOX",
                                   NULL,
                                   WS_CHILD | WS_VISIBLE | WS_BORDER | style,
                                   x,
                                   y,
                                   width,
                                   height,
                                   window->handle,
                                   NULL,
                                   window->appHandle,
                                   NULL);

    // Make sure we set the function and ID.
    child->eventFunction = eventFunction;
    child->data = data;

    SendMessage(child->handle, WM_SETFONT, (WPARAM)window->font, MAKELPARAM(FALSE, 0));

    return child;
}

bool comboBoxAddString(Child *child, const char *string)
{
    return SendMessage(child->handle, CB_ADDSTRING, 0, (LONG_PTR)string) != CB_ERR;
}

bool comboBoxDeleteString(Child *child, int index)
{
    if (index < 0 || index >= comboBoxGetCount(child))
    {
        return false;
    }

    return SendMessage(child->handle, CB_DELETESTRING, index, 0) != CB_ERR;
}


int comboBoxGetCount(Child *child)
{
    return SendMessage(child->handle, CB_GETCOUNT, 0, 0);
}
