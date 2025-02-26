#include "windowProcFunc.h"
#include "Window.h"
#include "Window_external.h"
#include <commctrl.h>
#include <stdio.h>

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

// This enum is so the WM_COMMAND switch case is easier to understand.
enum
{
    MENU,
    ACCELERATOR,
    CONTROL
};

// This are declared and defined here, because it is called when the WM_CLOSE message is received.
static void windowDestroy(void *windowIn);
// This executes the eventFunction associated with the handle.
static LRESULT handleEventFromControl(Window *window, WPARAM wParam, HWND controlHandle);

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
            // wParam is checked to see what the command is coming from.
            switch (HIWORD(wParam))
            {
                case MENU:
                {
                    // Get the ID of the menu that sent the message.
                    int menuId = LOWORD(wParam);

                    // Grab the size of the array.
                    size_t menuEventSize = dynamicArrayGetSize(window->menuEvents);
                    for (size_t i = 0; i < menuEventSize; i++)
                    {
                        MenuEvent *event = dynamicArrayGet(window->menuEvents, i);
                        // Don't pay attention to this.
                        if (event->id != menuId)
                        {
                            continue;
                        }
                        else if (event->id == menuId && event->eventFunction)
                        {
                            (*event->eventFunction)(window, wParam, event->data);
                            break;
                        }
                    }
                    return 0;
                }
                break;

                case ACCELERATOR:
                {
                    return 0;
                }
                break;

                case CONTROL:
                {
                    return handleEventFromControl(window, wParam, (HWND)lParam);
                }
                break;
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
    dynamicArrayDestroy(window->text);
    // Release the device context.
    ReleaseDC(window->handle, window->context);
    // Destroy the window.
    DestroyWindow(window->handle);
    // Finally free the window.
    free(window);
}

static LRESULT handleEventFromControl(Window *window, WPARAM wParam, HWND controlHandle)
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
        if (!child || !child->eventFunction)
        {
            continue;
        }
        else if (child->handle == controlHandle && child->eventFunction) // Double check the function before segfault.
        {
            // Execute the function and break the loop.
            (*child->eventFunction)(window, wParam, child->data);
            break;
        }
    }
    return 0;
}
