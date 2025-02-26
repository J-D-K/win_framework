#pragma once
#include <windows.h>

#ifdef __WINDOW_INTERNAL__
struct Menu
{
    // Windows menu handle.
    HMENU menu;
    // Position for adding menus to the menu.
    UINT position;
};
#endif
