#include "Window.h"
#include "DynamicArray.h"
#include "display.h"
#include "windowProcFunc.h"
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define __WINDOW_INTERNAL__
#include "Menu_internal.h"
#include "Window_internal.h"

// Declarations. Definitions at bottom of the file.
static void textDestroy(void *text);

Window *windowCreate(const char *windowClass,
                     const char *title,
                     int x,
                     int y,
                     int width,
                     int height,
                     DWORD style,
                     COLORREF windowColor,
                     Menu *menu,
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

    // Check if we need to center the window.
    if (x == WINDOW_CENTER)
    {
        x = (displayGetWidth() / 2) - (width / 2);
    }

    if (y == WINDOW_CENTER)
    {
        y = (displayGetHeight() / 2) - (height / 2);
    }

    // Create window.
    window->handle = CreateWindowEx(0,
                                    windowClass,
                                    title,
                                    style,
                                    x,
                                    y,
                                    width,
                                    height,
                                    parent == NULL ? NULL : parent->handle,
                                    menu == NULL ? NULL : menu->menu,
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

    // Allocate child, menu events, and text.
    window->children = dynamicArrayCreate(sizeof(Child), NULL);
    window->menuEvents = dynamicArrayCreate(sizeof(MenuEvent), NULL);
    window->tabWindows = dynamicArrayCreate(sizeof(Window *), NULL);
    window->text = dynamicArrayCreate(sizeof(Text), textDestroy);

    return window;
}

void windowClose(Window *window)
{
    SendMessage(window->handle, WM_CLOSE, 0, 0);
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

bool windowUpdate(Window *window)
{
    // Message count
    int messageCount = 0;
    // Message
    MSG message;
    while ((messageCount = GetMessage(&message, window->handle, 0, 0)) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    return messageCount > 0;
}

bool windowAddText(Window *window, int x, int y, const char *text)
{
    // Need this check so children can't be passed here.
    if (!window->text)
    {
        return false;
    }

    // Allocate a new text struct.
    Text *newText = dynamicArrayNew(window->text);
    if (!newText)
    {
        return false;
    }

    // Get string length and allocate buffer for it.
    size_t textLength = strlen(text);
    newText->text = malloc(textLength + 1);
    if (!newText->text)
    {
        // Erase the back.
        dynamicArrayErase(window->text, dynamicArrayGetSize(window->text) - 1);
        return false;
    }

    // Clear the text.
    memset(newText->text, 0x00, textLength + 1);

    // Copy the incoming text to it.
    memcpy(newText->text, text, textLength);

    // Make sure we have the right length.
    newText->length = strlen(newText->text);

    // Record X and Y.
    newText->x = x;
    newText->y = y;

    return true;
}

bool windowAddMenuEvent(Window *window, int menuId, EventFunction eventFunction, void *data)
{
    // Create a new event
    MenuEvent *menuEvent = dynamicArrayNew(window->menuEvents);
    if (!menuEvent)
    {
        return false;
    }
    // Set shit
    menuEvent->id = menuId;
    menuEvent->eventFunction = eventFunction;
    menuEvent->data = data;

    return true;
}

HWND windowGetHandle(Window *window)
{
    return window->handle;
}

HBRUSH windowGetBackground(Window *window)
{
    return window->backgroundColor;
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
