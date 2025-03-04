#include "Edit.h"
#include "Child.h"
#include <commctrl.h>

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

// Border color
COLORREF g_editBorderColor;
// Background color.
COLORREF g_editBackgroundColor;
// Text color.
COLORREF g_editTextColor;

// Declaration here. Definition later.
static LRESULT editDrawProc(HWND handle,
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam,
                            UINT_PTR subClassId,
                            DWORD_PTR refData);

Child *windowAddEdit(Window *window, int x, int y, int width, int height, DWORD style)
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

    // Create a single lined edit input.
    child->handle = CreateWindowEx(0,
                                   WC_EDITA,
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

    childInitFunctionsDefault(child);

    // Set the subclass.
    SetWindowSubclass(child->handle, editDrawProc, 0, 0);

    // Set the font.
    SendMessage(child->handle, WM_SETFONT, (WPARAM)window->font, MAKELPARAM(FALSE, 0));

    return child;
}

void editSetBorderColor(COLORREF color)
{
    g_editBorderColor = color;
}

void editSetBackgroundColor(COLORREF color)
{
    g_editBackgroundColor = color;
}

void editSetTextColor(COLORREF color)
{
    g_editTextColor = color;
}

int editGetTextLength(Child *child)
{
    // One is added because Windows doesn't count the NULL terminator.
    return SendMessage(child->handle, WM_GETTEXTLENGTH, 0, 0) + 1;
}

int editGetLineIndex(Child *child, int index)
{
    return SendMessage(child->handle, EM_LINEINDEX, index, 0);
}

int editGetLineCount(Child *child)
{
    return SendMessage(child->handle, EM_GETLINECOUNT, 0, 0);
}

int editGetLineLength(Child *child, int index)
{
    // MS's documentation of this is kind of confusing. It says it gets the length of the line passed as WPARAM,
    // but also says it's the index of the character? I guess you need to get the line index first?
    return SendMessage(child->handle, EM_LINELENGTH, index, 0);
}

bool editGetText(Child *child, char *buffer, size_t bufferSize)
{
    // Get the length first before continuing.
    size_t textLength = SendMessage(child->handle, WM_GETTEXTLENGTH, 0, 0);
    if (textLength > bufferSize)
    {
        return false;
    }
    // One is subtracted to account for the NULL terminator, which Windows doesn't count?
    return SendMessage(child->handle, WM_GETTEXT, bufferSize, (LONG_PTR)buffer) == textLength - 1;
}

bool editGetLine(Child *child, int index, char *buffer, int bufferSize)
{
    if (bufferSize < editGetLineLength(child, index))
    {
        return false;
    }
    return SendMessage(child->handle, EM_GETLINE, index, (LONG_PTR)buffer) > 0;
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

static LRESULT editDrawProc(HWND handle,
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam,
                            UINT_PTR subClassId,
                            DWORD_PTR refData)
{
    switch (message)
    {
        case WM_PAINT:
        {
            // Begin paint.
            PAINTSTRUCT paintStruct;
            HDC context = BeginPaint(handle, &paintStruct);

            // Select the pen and object to draw the rectangle.
            SelectObject(context, CreatePen(PS_SOLID, 0, g_editBorderColor));
            SelectObject(context, CreateSolidBrush(g_editBackgroundColor));

            // Draw the rectangle
            Rectangle(context,
                      paintStruct.rcPaint.left,
                      paintStruct.rcPaint.top,
                      paintStruct.rcPaint.right,
                      paintStruct.rcPaint.bottom);

            DefSubclassProc(handle, WM_PAINT, (WPARAM)context, lParam);

            EndPaint(handle, &paintStruct);

            return 0;
        }
        break;
    }
    return DefSubclassProc(handle, message, wParam, lParam);
}
