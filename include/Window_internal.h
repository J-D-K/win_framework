#pragma once

#ifdef __WINDOW_INTERNAL__
#include "DynamicArray.h"
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
    // Window handle.
    HWND handle;
    // Main application handle.
    HINSTANCE appHandle;
    // Window context.
    HDC context;
    // Window font. To do: Have more than one and force comic sans as one of them no matter what.
    HFONT font;
    // Window color.
    HBRUSH backgroundColor;
    // Text color.
    COLORREF textColor;
    // Dynamic array to hold children. Both the main and children have this.
    DynamicArray *children;
    // Array to hold text to draw to screen. Children do not have this.
    DynamicArray *text;
    // Only Child windows use this and the void *
    EventFunction eventFunction;
    // Data passed to ^
    void *data;
};

#endif
