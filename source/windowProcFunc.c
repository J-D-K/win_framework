#include "windowProcFunc.h"
#include "Window.h"
#include "Window_external.h"
#include <commctrl.h>
#include <stdio.h>

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

// This are declared and defined here, because it is called when the WM_CLOSE message is received.
static void windowDestroy(void *windowIn);

LRESULT windowProcFunc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Grab the pointer to our window that we set when it was created.
    Window *window = (Window *)GetWindowLongPtr(handle, GWLP_USERDATA);

    // Handle the message.
    switch (message)
    {
        case WM_CLOSE:
        {
            windowDestroy(window);
            return 0;
        }
        break;

        case WM_COMMAND:
        {
            // Just in case...
            if (!window->children)
            {
                return 0;
            }
            // Loop through the children, find which one sent the message and execute its EventFunction if it has one.
            size_t childCount = dynamicArrayGetSize(window->children);
            for (size_t i = 0; i < childCount; i++)
            {
                Child *child = dynamicArrayGet(window->children, i);
                if (!child || child->handle != (HWND)lParam)
                {
                    continue;
                }
                // If the child has an event function defined, execute it.
                if (child->eventFunction)
                {
                    (*child->eventFunction)(window, wParam, child->data);
                }
            }
        }
        break;

        case WM_CTLCOLORBTN:
        case WM_CTLCOLORSTATIC:
        {
            // This is needed to make sure read-only edit controls don't get colored weird.
            char className[32] = {0};
            GetClassName((HWND)lParam, className, 32);
            if (strcmp(className, "Edit") == 0)
            {
                SetBkMode((HDC)wParam, OPAQUE);
                return (LRESULT)CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF));
            }
            else
            {
                SetBkMode((HDC)wParam, TRANSPARENT);
                return (LRESULT)window->backgroundColor;
            }
        }
        break;

        case WM_PAINT:
        {
            // Just in case.
            if (!window->text)
            {
                return 0;
            }
            // All this really does is loop through and draw all the text structs to the screen.
            size_t textCount = dynamicArrayGetSize(window->text);
            for (size_t i = 0; i < textCount; i++)
            {
                Text *text = dynamicArrayGet(window->text, i);
                if (!text)
                {
                    continue;
                }
                TextOut(window->context, text->x, text->y, text->text, text->length);
            }
        }
        break;
    }
    return DefWindowProc(handle, message, wParam, lParam);
}

static void windowDestroy(void *windowIn)
{
    if (!windowIn)
    {
        return;
    }
    // Cast to window.
    Window *window = (Window *)windowIn;
    // Free children and text.
    dynamicArrayDestroy(window->children);
    dynamicArrayDestroy(window->text);
    // Release the device context.
    ReleaseDC(window->handle, window->context);
    // Destroy the window.
    DestroyWindow(window->handle);
    // Finally free the window.
    free(window);
}
