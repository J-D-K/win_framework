#pragma once

#ifdef __WINDOW_INTERNAL__
#include "DynamicArray.h"

// Child struct because they don't need all the the extra stuff a full window does.
struct Child
{
    // Child handle.
    HWND handle;
    // Function to execute when an event occurs.
    EventFunction commandFunction;
    // Data pointer that is passed to ^
    void *commandData;
    // Function for when WM_NOTIFY is processed.
    EventFunction notifyFunction;
    // Data pointer for notify.
    void *notifyData;
};

// This is the struct for menu events. I wasn't sure how else to add them?
typedef struct
{
    // The id of the menu with the event.
    int id;
    // Function to call when WM_COMMAND is processed.
    EventFunction eventFunction;
    // Data pointer for command function.
    void *data;
} MenuEvent;

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
    // Dynamic array to hold children.
    DynamicArray *children;
    // Dynamic array to hold menu events.
    DynamicArray *menuEvents;
    // Dynamic array to hold tab windows.
    DynamicArray *tabWindows;
    // Array to hold text to draw to screen. Children do not have this.
    DynamicArray *text;
};

#endif
