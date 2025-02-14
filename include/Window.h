#pragma once
#include "Button.h"
#include "ComboBox.h"
#include "Edit.h"
#include "ProgressBar.h"
#include <stdbool.h>
#include <windows.h>
// This needs to be here. Seems to conflict with windows.h
#include "Window_external.h"
#include <commctrl.h>

#ifdef __cplusplus
extern "C"
{
#endif
    /// @brief Creates a new window.
    /// @param windowClass Window class.
    /// @param title Window title.
    /// @param width Width of the window.
    /// @param height Height of the window.
    /// @param windowColor Background color of window
    /// @param handle Application handle.
    /// @return New window on success. NULL on failure.
    /// @note The window and all its children will automatically be freed when the message WM_QUIT is posted to it.
    Window *windowCreate(const char *windowClass,
                         const char *title,
                         int width,
                         int height,
                         COLORREF windowColor,
                         HINSTANCE handle);

    /// @brief Creates and sets the system font.
    /// @param window Window to set the font for.
    /// @param font Name of the font.
    /// @param size Size of the font.
    void windowSetFont(Window *window, const char *fontName, int size);

    /// @brief Sets the color used for drawing text.
    /// @param window Window to set color for.
    /// @param color Color to set text to.
    void windowSetTextColor(Window *window, COLORREF color);

    /// @brief Shows the window.
    /// @param window Window to show.
    void windowShow(Window *window);

    /// @brief Runs the update routine for the window.
    /// @param window Window to update and handle.
    /// @note WM_DESTROY is not handled and PostQuitMessage is not used to allow the program to free resources correctly.
    bool windowUpdate(Window *window);

    /// @brief Adds text to be draw to the window.
    /// @param window Window to add text to.
    /// @param x X coordinate.
    /// @param y Y coordinate.
    /// @param text Text to draw.
    void windowAddText(Window *window, int x, int y, const char *text);

    /// @brief Returns the window handle.
    /// @param window Window to get the handle of.
    /// @return Window's handle?
    HWND windowGetHandle(Window *window);

#ifdef __cplusplus
}
#endif
