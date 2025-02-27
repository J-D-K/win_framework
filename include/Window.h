#pragma once
#include "Button.h"
#include "ComboBox.h"
#include "Edit.h"
#include "ListBox.h"
#include "Menu.h"
#include "ProgressBar.h"
#include "Tab.h"
#include "display.h"
#include "filename.h"
#include <stdbool.h>
#include <windows.h>
// This needs to be here. Seems to conflict with windows.h
#include "Menu_external.h"
#include "Window_external.h"
#include <commctrl.h>

/// @brief Used to center the main window.
static const int WINDOW_CENTER = -1;
/// @brief This is the default style I use for creating the main window.
static const int WINDOW_MAIN_DEFAULT_STYLE = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;

#ifdef __cplusplus
extern "C"
{
#endif
    /// @brief Creates a new window.
    /// @param windowClass Window class.
    /// @param title Window title.
    /// @param x X position of window.
    /// @param y Y position of window.
    /// @param width Width of the window.
    /// @param height Height of the window.
    /// @param style Style of the window.
    /// @param windowColor Background color of window
    /// @param menu Menu for window (if wanted).
    /// @param parent Parent of the window. Pass NULL if creating the mainWindow.
    /// @param appHandle Application handle.
    /// @return New window on success. NULL on failure.
    /// @note The window and all its children will automatically be freed when the message WM_QUIT is posted to it.
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
                         HINSTANCE appHandle);

    /// @brief Sends the WM_CLOSE message to the window.
    /// @param window Window to send the message to.
    void windowClose(Window *window);

    /// @brief Loads and sets the big icon of the window.
    /// @param window Window to set the icon for.
    /// @param resource Int value of resource in resources file.
    void windowSetBigIcon(Window *window, int resource);

    /// @brief Loads and sets the small icon of the window.
    /// @param window Window to set the icon for.
    /// @param resource Int value of resource in resources file.
    void windowSetSmallIcon(Window *window, int resource);

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
    bool windowAddText(Window *window, int x, int y, const char *text);

    /// @brief Adds a menu event to the window.
    /// @param window Window to add event to.
    /// @param menuId ID of the menu item.
    /// @param eventFunction Function that's executed when the item with menuId is clicked.
    /// @param data Data to pass to the event function.
    bool windowAddMenuEvent(Window *window, int menuId, EventFunction eventFunction, void *data);

    /// @brief Returns the window handle.
    /// @param window Window to get the handle of.
    /// @return Window's handle?
    HWND windowGetHandle(Window *window);

    /// @brief Returns the brush created to draw the window's background.
    /// @param window Window to get the brush of.
    /// @return HBRUSH window color.
    HBRUSH windowGetBackground(Window *window);

#ifdef __cplusplus
}
#endif
