#include "Window.h"
#include "DynamicArray.h"
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// Child window.
typedef struct
{
    // Child's handle.
    HWND handle;
    // Function to call when event occurs.
    EventFunction eventFunction;
    // Data to pass to eventFunction.
    void *data;
    // Child's ID.
    int id;
} Child;

// Text struct so the window can draw the text on WM_PAINT.
typedef struct
{
    // X and Y coordinates.
    int x, y, length;
    // Text buffer.
    char *text;
} Text;

// Actual window.
struct Window
{
    // Main window handle.
    HWND handle;
    // Main application handle.
    HINSTANCE appHandle;
    // Window context.
    HDC context;
    // Window font.
    HFONT font;
    // Dynamic array to hold children.
    DynamicArray *children;
    // Array to hold text to draw to screen.
    DynamicArray *text;
    // Keep track of children.
    int childId;
};

// Declarations. Definitions at bottom of the file.
static void windowDestroy(Window *window);
static void textDestroy(void *text);
static Child *findChildById(DynamicArray *children, int id);

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
        size_t childCount = dynamicArrayGetSize(window->children);
        for (size_t i = 0; i < childCount; i++)
        {
            Child *child = dynamicArrayGet(window->children, i);
            if (!child)
            {
                break;
            }
            else if (child->handle != (HWND)lParam)
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
                     const char *font,
                     COLORREF windowColor,
                     COLORREF textColor,
                     HINSTANCE handle)
{
    // Try to allocate window first.
    Window *window = malloc(sizeof(Window));
    if (!window)
    {
        return NULL;
    }

    // Create and register class.
    WNDCLASSEX classEx = {.cbSize = sizeof(WNDCLASSEX),
                          .style = 0,
                          .lpfnWndProc = windowEventHandler,
                          .cbClsExtra = 0,
                          .cbWndExtra = 0,
                          .hInstance = handle,
                          .hIcon = NULL,
                          .hCursor = NULL,
                          .hbrBackground = (HBRUSH)CreateSolidBrush(windowColor),
                          .lpszMenuName = NULL,
                          .lpszClassName = windowClass,
                          .hIconSm = NULL};
    if (!RegisterClassEx(&classEx))
    {
        free(window);
        return NULL;
    }

    // Create window.
    window->handle = CreateWindowEx(WS_EX_CLIENTEDGE,
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

    // Load font.
    window->font = CreateFont(16,
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
                              font);
    // Set font
    window->context = GetDC(window->handle);
    SelectObject(window->context, window->font);
    // Set transparency & text color.
    SetTextColor(window->context, textColor);
    SetBkColor(window->context, windowColor);

    // Set the pointer for this window handle to pass the window struct.
    SetWindowLongPtr(window->handle, GWLP_USERDATA, (LONG_PTR)window);

    // Allocate child and text arrays.
    window->children = dynamicArrayCreate(sizeof(Child), NULL);
    window->text = dynamicArrayCreate(sizeof(Text), textDestroy);
    window->childId = 0;

    return window;
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

int windowAddTextInput(Window *window, int x, int y, int width, bool readonly, EventFunction eventFunction, void *data)
{
    // Allocate new child.
    Child *child = dynamicArrayNew(window->children);
    if (!child)
    {
        return -1;
    }

    // Get text metrics for height.
    TEXTMETRIC textMetrics;
    GetTextMetrics(window->context, &textMetrics);

    // Style.
    DWORD dwStyle = WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL | (readonly ? ES_READONLY : 0);

    // Create a single lined edit input.
    child->handle = CreateWindowEx(0,
                                   "EDIT",
                                   NULL,
                                   dwStyle,
                                   x,
                                   y,
                                   width,
                                   textMetrics.tmHeight + 4,
                                   window->handle,
                                   NULL,
                                   window->appHandle,
                                   NULL);

    // Make sure we set the function and ID.
    child->eventFunction = eventFunction;
    child->data = data;
    child->id = window->childId++;

    // Set the font.
    SendMessage(child->handle, WM_SETFONT, (WPARAM)window->font, MAKELPARAM(FALSE, 0));

    // Return the ID.
    return child->id;
}

int windowAddMultilineTextInput(Window *window,
                                int x,
                                int y,
                                int width,
                                int height,
                                bool readonly,
                                EventFunction eventFunction,
                                void *data)
{
    // Allocate new child.
    Child *child = dynamicArrayNew(window->children);
    if (!child)
    {
        return -1;
    }

    // Get text metrics for height.
    TEXTMETRIC textMetrics;
    GetTextMetrics(window->context, &textMetrics);

    // Style.
    DWORD dwStyle = WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_VSCROLL | ES_MULTILINE |
                    (readonly ? ES_READONLY : 0);

    // Create a single lined edit input.
    child->handle =
        CreateWindowEx(0, "EDIT", NULL, dwStyle, x, y, width, height, window->handle, NULL, window->appHandle, NULL);

    // Make sure we set the function and ID.
    child->eventFunction = eventFunction;
    child->data = data;
    child->id = window->childId++;

    // Set the font.
    SendMessage(child->handle, WM_SETFONT, (WPARAM)window->font, MAKELPARAM(FALSE, 0));

    // Return the ID.
    return child->id;
}

int windowAddPasswordInput(Window *window, int x, int y, int width, EventFunction eventFunction, void *data)
{
    // Allocate new child.
    Child *child = dynamicArrayNew(window->children);
    if (!child)
    {
        return -1;
    }

    // Get text metrics for height.
    TEXTMETRIC textMetrics;
    GetTextMetrics(window->context, &textMetrics);

    // Create a single lined edit input.
    child->handle = CreateWindowEx(0,
                                   "EDIT",
                                   NULL,
                                   WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL | ES_PASSWORD,
                                   x,
                                   y,
                                   width,
                                   textMetrics.tmHeight + 4,
                                   window->handle,
                                   NULL,
                                   window->appHandle,
                                   NULL);

    // Make sure we set the function and ID.
    child->eventFunction = eventFunction;
    child->data = data;
    child->id = window->childId++;

    // Set the font.
    SendMessage(child->handle, WM_SETFONT, (WPARAM)window->font, MAKELPARAM(FALSE, 0));

    // Return the ID.
    return child->id;
}

int windowAddButton(Window *window,
                    int x,
                    int y,
                    int width,
                    const char *buttonText,
                    EventFunction eventFunction,
                    void *data)
{
    Child *child = dynamicArrayNew(window->children);
    if (!child)
    {
        return -1;
    }

    // Need this to calculate button height.
    TEXTMETRIC textMetrics;
    GetTextMetrics(window->context, &textMetrics);

    // Create the button.
    child->handle = CreateWindowEx(0,
                                   "BUTTON",
                                   buttonText,
                                   WS_CHILD | WS_BORDER | WS_VISIBLE,
                                   x,
                                   y,
                                   width,
                                   textMetrics.tmHeight + 4,
                                   window->handle,
                                   NULL,
                                   window->appHandle,
                                   NULL);

    child->eventFunction = eventFunction;
    child->data = data;
    child->id = window->childId++;

    SendMessage(child->handle, WM_SETFONT, (WPARAM)window->font, MAKELPARAM(FALSE, 0));

    return child->id;
}

HWND windowGetHandle(Window *window)
{
    return window->handle;
}

bool windowGetTextFromInput(Window *window, int id, char *buffer, size_t bufferSize)
{
    // Get the child by the ID. Bail if it's not found.
    Child *child = findChildById(window->children, id);
    if (!child)
    {
        return false;
    }

    // Get the length first before continuing.
    size_t textLength = SendMessage(child->handle, WM_GETTEXTLENGTH, 0, 0);
    if (textLength + 1 >= bufferSize)
    {
        return false;
    }

    return SendMessage(child->handle, WM_GETTEXT, bufferSize, (LONG_PTR)buffer) == textLength;
}

bool windowSetInputText(Window *window, int id, const char *text)
{
    Child *child = findChildById(window->children, id);
    if (!child)
    {
        return false;
    }
    // This is easy.
    return SendMessage(child->handle, WM_SETTEXT, 0, (LONG_PTR)text) == TRUE;
}

bool windowAppendInputText(Window *window, int id, const char *text)
{
    Child *child = findChildById(window->children, id);
    if (!child)
    {
        return false;
    }
    // First we need to make sure the cursor is at the end. The -1 should rollover to the end.
    SendMessage(child->handle, EM_SETSEL, -1, -1);
    // Now we use replace to append it. I'm not sure how else to do this without having to read and append it and that seems stupid.
    SendMessage(child->handle, EM_REPLACESEL, FALSE, (LONG_PTR)text);
}

static void windowDestroy(Window *window)
{
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

// This searches for a child by the ID passed.
static Child *findChildById(DynamicArray *children, int id)
{
    Child *child = NULL;
    size_t arraySize = dynamicArrayGetSize(children);
    for (size_t i = 0; i < arraySize; i++)
    {
        Child *subChild = dynamicArrayGet(children, i);
        if (subChild->id == id)
        {
            child = subChild;
            break;
        }
    }
    return child;
}
