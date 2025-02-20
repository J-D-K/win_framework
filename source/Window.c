#include "Window.h"
#include "DynamicArray.h"
#include "windowProcFunc.h"
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

// Declarations. Definitions at bottom of the file.
static void textDestroy(void *text);

Window *windowCreate(const char *windowClass,
                     const char *title,
                     int width,
                     int height,
                     DWORD style,
                     COLORREF windowColor,
                     Window *parent,
                     HINSTANCE appHandle)
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
                          .lpfnWndProc = windowProcFunc,
                          .cbClsExtra = 0,
                          .cbWndExtra = 0,
                          .hInstance = appHandle,
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
                                    style,
                                    CW_USEDEFAULT,
                                    CW_USEDEFAULT,
                                    width,
                                    height,
                                    parent == NULL ? NULL : parent->handle,
                                    NULL,
                                    appHandle,
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
    window->children = dynamicArrayCreate(sizeof(Child), NULL);
    window->text = dynamicArrayCreate(sizeof(Text), textDestroy);

    return window;
}

void windowSetBigIcon(Window *window, int resource)
{
    HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(resource));
    if (!icon)
    {
        return;
    }

    SendMessage(window->handle, WM_SETICON, 0, (LPARAM)icon);
}

void windowSetSmallIcon(Window *window, int resource)
{
    HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(resource));
    if (!icon)
    {
        return;
    }

    SendMessage(window->handle, WM_SETICON, 1, (LPARAM)icon);
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

void windowHide(Window *window)
{
    ShowWindow(window->handle, SW_HIDE);
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

HWND windowGetHandle(Window *window)
{
    return window->handle;
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
