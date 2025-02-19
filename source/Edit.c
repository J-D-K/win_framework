#include "Edit.h"

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

Child *windowAddEdit(Window *window,
                     int x,
                     int y,
                     int width,
                     int height,
                     DWORD style,
                     EventFunction eventFunction,
                     void *data)
{
    // Children check
    if (!window->children)
    {
        return NULL;
    }

    // Allocate new child.
    Child *child = dynamicArrayNew(window->children);
    if (!child)
    {
        return NULL;
    }

    if (height == AUTO_SIZE)
    {
        // Get text metrics for height.
        TEXTMETRIC textMetrics;
        GetTextMetrics(window->context, &textMetrics);

        height = textMetrics.tmHeight + 4;
    }

    // Style.
    DWORD dwStyle = WS_CHILD | WS_BORDER | WS_VISIBLE | style;

    // Create a single lined edit input.
    child->handle =
        CreateWindowEx(0, "EDIT", NULL, dwStyle, x, y, width, height, window->handle, NULL, window->appHandle, NULL);

    // Make sure we set the function and ID.
    child->eventFunction = eventFunction;
    child->data = data;

    // Set the font.
    SendMessage(child->handle, WM_SETFONT, (WPARAM)window->font, MAKELPARAM(FALSE, 0));

    return child;
}

int editGetTextLength(Child *child)
{
    return SendMessage(child->handle, WM_GETTEXTLENGTH, 0, 0);
}

bool editGetText(Child *child, char *buffer, size_t bufferSize)
{
    // Get the length first before continuing.
    size_t textLength = SendMessage(child->handle, WM_GETTEXTLENGTH, 0, 0);
    if (textLength + 1 > bufferSize)
    {
        return false;
    }
    return SendMessage(child->handle, WM_GETTEXT, bufferSize, (LONG_PTR)buffer) == textLength;
}

bool editSetText(Child *child, const char *text)
{
    return SendMessage(child->handle, WM_SETTEXT, 0, (LONG_PTR)text) == TRUE;
}

void editAppendText(Child *child, const char *text)
{
    // First we need to make sure the cursor is at the end. The -1 should rollover to the end.
    SendMessage(child->handle, EM_SETSEL, -1, -1);
    // Now we use replace to append it. I'm not sure how else to do this without having to read and append it and that seems stupid.
    SendMessage(child->handle, EM_REPLACESEL, FALSE, (LONG_PTR)text);
}
