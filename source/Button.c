#include "Button.h"
#include "Child.h"
#include <commctrl.h>

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

// Colors used to draw buttons.
COLORREF g_buttonBorderColor;
COLORREF g_buttonBackgroundColor;
COLORREF g_buttonTextColor;

// Declarations here. Definitions later.
static LRESULT buttonSubProc(HWND handle,
                             UINT message,
                             WPARAM wParam,
                             LPARAM lParam,
                             UINT_PTR subClass,
                             DWORD_PTR refData);

Child *windowAddButton(Window *window, int x, int y, int width, int height, const char *buttonText, DWORD style)
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
                                   WC_BUTTONA,
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

    childInitFunctionsDefault(child);

    SetWindowSubclass(child->handle, buttonSubProc, 0, 0);

    SendMessage(child->handle, WM_SETFONT, (WPARAM)window->font, 0);

    return child;
}

void buttonSetBorderColor(COLORREF color)
{
    g_buttonBorderColor = color;
}

void buttonSetBackgroundColor(COLORREF color)
{
    g_buttonBackgroundColor = color;
}

void buttonSetTextColor(COLORREF color)
{
    g_buttonTextColor = color;
}

int buttonGetWidth(Child *child)
{
    // Rect to get coords.
    RECT buttonRect = {0};

    if (GetWindowRect(child->handle, &buttonRect) == FALSE)
    {
        return -1;
    }
    return buttonRect.right - buttonRect.left;
}

int buttonGetHeight(Child *child)
{
    RECT buttonRect = {0};

    if (GetWindowRect(child->handle, &buttonRect) == FALSE)
    {
        return -1;
    }
    return buttonRect.bottom - buttonRect.top;
}

bool buttonSetIdealSize(Child *child)
{
    // Struct to get the size.
    SIZE buttonSize = {0};
    if (SendMessage(child->handle, BCM_GETIDEALSIZE, 0, (LONG_PTR)&buttonSize) == FALSE)
    {
        return false;
    }
    // I guess this is the way to do this?
    return SetWindowPos(child->handle, NULL, 0, 0, buttonSize.cx, buttonSize.cy, SWP_NOMOVE) == TRUE;
}

bool buttonGetCheck(Child *child)
{
    return SendMessage(child->handle, BM_GETCHECK, 0, 0) == BST_CHECKED;
}

void buttonSetCheck(Child *child, bool check)
{
    SendMessage(child->handle, BM_SETCHECK, check ? BST_CHECKED : BST_UNCHECKED, 0);
}

static LRESULT buttonSubProc(HWND handle,
                             UINT message,
                             WPARAM wParam,
                             LPARAM lParam,
                             UINT_PTR subClass,
                             DWORD_PTR refData)
{
    switch (message)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT paintStruct;
            HDC context = BeginPaint(handle, &paintStruct);

            // Grab the font
            HFONT buttonFont = (HFONT)SendMessage(handle, WM_GETFONT, 0, 0);

            // Create/select pen, brush, and font.
            SelectObject(context, CreatePen(PS_SOLID, 0, g_buttonBorderColor));
            SelectObject(context, CreateSolidBrush(g_buttonBackgroundColor));
            SelectObject(context, buttonFont);

            // Set the context
            SetBkColor(context, g_buttonBackgroundColor);
            SetTextColor(context, g_buttonTextColor);

            // Draw the rectangle.
            Rectangle(context,
                      paintStruct.rcPaint.left,
                      paintStruct.rcPaint.top,
                      paintStruct.rcPaint.right,
                      paintStruct.rcPaint.bottom);

            // I guess you need to draw the text yourself for buttons?
            int textLength = GetWindowTextLength(handle) + 1;
            char buttonText[textLength];
            memset(buttonText, 0x00, textLength);
            GetWindowText(handle, buttonText, textLength);

            // Text out and hope everything is OK!
            DrawText(context,
                     buttonText,
                     strlen(buttonText),
                     &paintStruct.rcPaint,
                     DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            EndPaint(handle, &paintStruct);
            return 0;
        }
        break;
    }
    return DefSubclassProc(handle, message, wParam, lParam);
}
