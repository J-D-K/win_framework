#include "Window.h"
#include "DynamicArray.h"
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

// Declarations. Definitions at bottom of the file.
static void windowDestroy(void *windowIn);
static void textDestroy(void *text);

// Function that processes window events.
static LRESULT windowEventHandler(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Get window pointer.
    Window *window = (Window *)GetWindowLongPtr(handle, GWLP_USERDATA);

    // Handle the message.
    switch (message)
    {
        case WM_PAINT:
        {
            // Draw text.
            size_t textCount = dynamicArrayGetSize(window->text);
            for (size_t i = 0; i < textCount; i++)
            {
                Text *text = dynamicArrayGet(window->text, i);
                if (!text)
                {
                    break;
                }
                TextOut(window->context, text->x, text->y, text->text, text->length);
            }
        }
        break;

        case WM_COMMAND:
        {
            // Just in case.
            if (!window->children)
            {
                return 0;
            }

            // Loop through array and execute the event function if there is one.
            size_t childCount = dynamicArrayGetSize(window->children);
            for (size_t i = 0; i < childCount; i++)
            {
                Window *child = dynamicArrayGet(window->children, i);
                if (!child || child->handle != (HWND)lParam)
                {
                    continue;
                }
                // Execute the function.
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
            // This is needed so read-only edits don't get colored weird.
            char className[32] = {0}; // Class names can only be 32 chars long.
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

        case WM_CLOSE:
        {
            windowDestroy(window);
        }
        break;
    }
    return DefWindowProc(handle, message, wParam, lParam);
}

Window *windowCreate(const char *windowClass,
                     const char *title,
                     int width,
                     int height,
                     COLORREF windowColor,
                     HINSTANCE handle)
{
    // Try to allocate window first.
    Window *window = malloc(sizeof(Window));
    if (!window)
    {
        return NULL;
    }

    // Save these so shit looks right.
    window->backgroundColor = CreateSolidBrush(windowColor);

    // Create and register class.
    WNDCLASSEX classEx = {.cbSize = sizeof(WNDCLASSEX),
                          .style = 0,
                          .lpfnWndProc = windowEventHandler,
                          .cbClsExtra = 0,
                          .cbWndExtra = 0,
                          .hInstance = handle,
                          .hIcon = NULL,
                          .hCursor = NULL,
                          .hbrBackground = window->backgroundColor,
                          .lpszMenuName = NULL,
                          .lpszClassName = windowClass,
                          .hIconSm = NULL};
    if (!RegisterClassEx(&classEx))
    {
        free(window);
        return NULL;
    }

    // Create window.
    window->handle = CreateWindowEx(0,
                                    windowClass,
                                    title,
                                    WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
                                    CW_USEDEFAULT,
                                    CW_USEDEFAULT,
                                    width,
                                    height,
                                    NULL,
                                    NULL,
                                    handle,
                                    NULL);
    if (!window->handle)
    {
        free(window);
        return NULL;
    }
    // Set font and make sure it's rendered with transparency.
    window->context = GetDC(window->handle);
    SetBkMode(window->context, TRANSPARENT);

    // Set the pointer for this window handle to pass the window struct.
    SetWindowLongPtr(window->handle, GWLP_USERDATA, (LONG_PTR)window);

    // Allocate child and text arrays.
    window->children = dynamicArrayCreate(sizeof(Window), windowDestroy);
    window->text = dynamicArrayCreate(sizeof(Text), textDestroy);

    return window;
}

void windowSetFont(Window *window, const char *fontName, int size)
{
    // Load font.
    window->font = CreateFont(size,
                              0,
                              0,
                              0,
                              550,
                              FALSE,
                              FALSE,
                              FALSE,
                              DEFAULT_CHARSET,
                              OUT_DEFAULT_PRECIS,
                              CLIP_DEFAULT_PRECIS,
                              CLEARTYPE_NATURAL_QUALITY,
                              FF_DONTCARE,
                              fontName);

    SelectObject(window->context, window->font);
}

void windowSetTextColor(Window *window, COLORREF color)
{
    // Save the color.
    window->textColor = color;
    // Set it for the window.
    SetTextColor(window->context, color);
}

void windowShow(Window *window)
{
    ShowWindow(window->handle, SW_NORMAL);
    UpdateWindow(window->handle);
}

bool windowUpdate(Window *window)
{
    // Message
    MSG message;
    // Record this.
    int messageCount = GetMessage(&message, window->handle, 0, 0);
    TranslateMessage(&message);
    DispatchMessage(&message);
    // Return this so we know if the window is still open.
    return messageCount > 0;
}

void windowAddText(Window *window, int x, int y, const char *text)
{
    // Need this check so children can't be passed here.
    if (!window->text)
    {
        return;
    }

    // Allocate a new text struct.
    Text *newText = dynamicArrayNew(window->text);
    if (!newText)
    {
        return;
    }

    // Get string length and allocate buffer for it.
    size_t textLength = strlen(text);
    newText->text = malloc(textLength + 1);
    memset(newText->text, 0x00, textLength + 1);

    // Copy the incoming text to it.
    memcpy(newText->text, text, textLength);

    // Make sure we have the right length.
    newText->length = strlen(newText->text);

    // Record X and Y.
    newText->x = x;
    newText->y = y;
}


Window *windowAddButton(Window *window,
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

    Window *child = dynamicArrayNew(window->children);
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
    // NULL the rest for this.
    child->appHandle = NULL;
    child->children = NULL;
    child->context = GetDC(child->handle);
    // This might need to be carried over.
    child->font = window->font;
    child->text = NULL;
    // Make sure we set the function and ID.
    child->eventFunction = eventFunction;
    child->data = data;

    SendMessage(child->handle, WM_SETFONT, (WPARAM)window->font, MAKELPARAM(FALSE, 0));

    return child;
}

HWND windowGetHandle(Window *window)
{
    return window->handle;
}

// This frees the memory associated with window.
static void windowDestroy(void *windowIn)
{
    if (!windowIn)
    {
        return;
    }
    // Cast to window.
    Window *window = (Window *)windowIn;
    // Destroy text and children.
    dynamicArrayDestroy(window->text);
    dynamicArrayDestroy(window->children);
    // Release the context
    ReleaseDC(window->handle, window->context);
    // Destroy to window.
    DestroyWindow(window->handle);
    // Destroy main window.
    free(window);
}

// Function to free and destroy text structs;
static void textDestroy(void *text)
{
    Text *textIn = (Text *)text;
    if (textIn->text)
    {
        free(textIn->text);
    }
    free(textIn);
}
