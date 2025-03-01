#include "ComboBox.h"
#include "Child.h"
#include <commctrl.h>

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

// This checks if the index passed is valid.
static bool comboBoxIndexIsValid(Child *child, int index);

Child *windowAddComboxBox(Window *window, int x, int y, int width, int height, DWORD style)
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
                                   WC_COMBOBOXA,
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
    if (!child->handle)
    {
        return NULL;
    }

    childInitFunctionsDefault(child);

    SendMessage(child->handle, WM_SETFONT, (WPARAM)window->font, MAKELPARAM(FALSE, 0));

    return child;
}

bool comboBoxAddString(Child *child, const char *string)
{
    return SendMessage(child->handle, CB_ADDSTRING, 0, (LONG_PTR)string) != CB_ERR;
}

bool comboBoxInsertString(Child *child, int index, const char *string)
{
    return SendMessage(child->handle, CB_INSERTSTRING, index, (LONG_PTR)string) != CB_ERR;
}

bool comboBoxDeleteString(Child *child, int index)
{
    if (!comboBoxIndexIsValid(child, index))
    {
        return false;
    }
    return SendMessage(child->handle, CB_DELETESTRING, index, 0) != CB_ERR;
}

int comboBoxGetCount(Child *child)
{
    return SendMessage(child->handle, CB_GETCOUNT, 0, 0);
}

int comboBoxGetCurrentSelected(Child *child)
{
    return SendMessage(child->handle, CB_GETCURSEL, 0, 0);
}

int comboBoxGetStringLength(Child *child, int index)
{
    if (!comboBoxIndexIsValid(child, index))
    {
        return CB_ERR;
    }
    // Add one for NULL terminator.
    return SendMessage(child->handle, CB_GETLBTEXTLEN, index, 0) + 1;
}

bool comboBoxGetString(Child *child, int index, char *buffer, size_t bufferSize)
{
    if (!comboBoxIndexIsValid(child, index) || comboBoxGetStringLength(child, index) <= (int)bufferSize)
    {
        return false;
    }
    return SendMessage(child->handle, CB_GETLBTEXT, index, (LONG_PTR)buffer) != CB_ERR;
}

static bool comboBoxIndexIsValid(Child *child, int index)
{
    return index >= 0 && index < comboBoxGetCount(child);
}
