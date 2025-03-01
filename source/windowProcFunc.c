#include "windowProcFunc.h"
#include "Window.h"
#include "Window_external.h"
#include <commctrl.h>
#include <stdio.h>

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

// This are declared and defined here, because it is called when the WM_CLOSE message is received.
static void windowDestroy(void *windowIn);
// This handles menu events
static LRESULT handleMenuEvents(Window *window, WPARAM wParam, LPARAM lParam);
// This executes the eventFunction associated with the handle.
static LRESULT handleControlEvents(Window *window, WPARAM wParam, LPARAM lParam);

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
            // This is the only real way to be sure it's a menu event?
            if (HIWORD(wParam) == 0 && (HWND)lParam == NULL)
            {
                return handleMenuEvents(window, wParam, lParam);
            }
            else // This will also trigger with accelerators. Dunno if that's bad or not yet.
            {
                return handleControlEvents(window, wParam, lParam);
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
                // Set the background mode to opaque and return white for edit controls.
                SetBkMode((HDC)wParam, OPAQUE);
                return (LRESULT)CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF));
            }
            else
            {
                // Everything else gets transparent and the window's color.
                SetBkMode((HDC)wParam, TRANSPARENT);
                return (LRESULT)window->backgroundColor;
            }
        }
        break;

        case WM_NOTIFY:
        {
            // This is what tab controls use.
        }
        break;

        case WM_PAINT:
        {
            // If there isn't any text to draw, just break and let the default take over.
            if (!window->text)
            {
                break;
            }

            // Begin paint.
            PAINTSTRUCT paintStruct;
            BeginPaint(window->handle, &paintStruct);

            // Fill the background.
            FillRect(window->context, &paintStruct.rcPaint, window->backgroundColor);

            // All this really does is loop through and draws all the text structs to the screen.
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
            // End paint
            EndPaint(window->handle, &paintStruct);
            // This expects 0 returned.
            return 0;
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
    dynamicArrayDestroy(window->menuEvents);
    dynamicArrayDestroy(window->tabWindows);
    dynamicArrayDestroy(window->text);
    // Release the device context.
    ReleaseDC(window->handle, window->context);
    // Destroy the window.
    DestroyWindow(window->handle);
    // Finally free the window.
    free(window);
}

static LRESULT handleMenuEvents(Window *window, WPARAM wParam, LPARAM lParam)
{
    if (!window || !window->menuEvents)
    {
        return 0;
    }

    // Get menu event count.
    int events = dynamicArrayGetSize(window->menuEvents);

    for (int i = 0; i < events; i++)
    {
        // Grab the current
        MenuEvent *event = dynamicArrayGet(window->menuEvents, i);
        if (!event || event->id != LOWORD(wParam))
        {
            continue;
        }
        else if (event->eventFunction)
        {
            (*event->eventFunction)(window, wParam, lParam, event->data);
        }
    }
    return 0;
}

static LRESULT handleControlEvents(Window *window, WPARAM wParam, LPARAM lParam)
{
    if (!window || !window->children)
    {
        return 0;
    }

    // Get the size of the array.
    size_t childrenCount = dynamicArrayGetSize(window->children);

    // Loop through and find the control.
    for (size_t i = 0; i < childrenCount; i++)
    {
        Child *child = dynamicArrayGet(window->children, i);
        if (!child || !child->commandFunction)
        {
            continue;
        }
        else if (child->handle == (HWND)lParam && child->commandFunction) // Double check the function before segfault.
        {
            // Execute the function and break the loop.
            (*child->commandFunction)(window, wParam, lParam, child->commandData);
            break;
        }
    }
    return 0;
}
