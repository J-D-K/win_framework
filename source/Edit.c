#include "Edit.h"

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

Window *windowAddEdit(Window *window,
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
    Window *child = dynamicArrayNew(window->children);
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

    // NULL the rest for this.
    child->appHandle = NULL;
    child->context = NULL;
    child->font = window->font;
    child->children = NULL;
    child->text = NULL;
    // Make sure we set the function and ID.
    child->eventFunction = eventFunction;
    child->data = data;

    // Set the font.
    SendMessage(child->handle, WM_SETFONT, (WPARAM)window->font, MAKELPARAM(FALSE, 0));

    return child;
}

int editGetTextLength(Window *window)
{
    return SendMessage(window->handle, WM_GETTEXTLENGTH, 0, 0);
}

bool editGetText(Window *window, char *buffer, size_t bufferSize)
{
    // Get the length first before continuing.
    size_t textLength = SendMessage(window->handle, WM_GETTEXTLENGTH, 0, 0);
    if (textLength + 1 > bufferSize)
    {
        return false;
    }
    return SendMessage(window->handle, WM_GETTEXT, bufferSize, (LONG_PTR)buffer) == textLength;
}

bool editSetText(Window *window, const char *text)
{
    return SendMessage(window->handle, WM_SETTEXT, 0, (LONG_PTR)text) == TRUE;
}

void editAppendText(Window *window, const char *text)
{
    // First we need to make sure the cursor is at the end. The -1 should rollover to the end.
    SendMessage(window->handle, EM_SETSEL, -1, -1);
    // Now we use replace to append it. I'm not sure how else to do this without having to read and append it and that seems stupid.
    SendMessage(window->handle, EM_REPLACESEL, FALSE, (LONG_PTR)text);
}
