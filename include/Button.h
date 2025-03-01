#pragma once
#include "Window_external.h"

// This header contains functions related to buttons.

#ifdef __cplusplus
extern "C"
{
#endif

    /// @brief Adds a button to the
    /// @param window Window to add the button to.
    /// @param x X coordinate of the window.
    /// @param y Y coordinate of the window.
    /// @param width Width of the button.
    /// @param height Height of the button. Passing AUTO_HEIGHT will size this automatically according to font size.
    /// @param buttonText Text displayed on button.
    /// @param style Style of button to use.
    /// @param eventFunction Function to execute on button triggering and event.
    /// @return Handle of the window/child on success. NULL on failure.
    Child *windowAddButton(Window *window, int x, int y, int width, int height, const char *buttonText, DWORD style);

    /// @brief Sets the ideal size of a button according to Windows.
    /// @param child Button to set the size for.
    /// @return True on success. False on failure.
    bool buttonSetIdealSize(Child *child);

    /// @brief Returns the width of the button.
    /// @param child Button to get the width of.
    /// @return Width of the button.
    int buttonGetWidth(Child *child);

    /// @brief Returns the height of the button.
    /// @param child Button to get the height of.
    /// @return Height of the button.
    int buttonGetHeight(Child *child);

    /// @brief Returns if the window passed is check (RADIOBUTTON and CHECKBOX)
    /// @param child Child window to check.
    /// @return True if checked. False if not.
    bool buttonGetCheck(Child *child);

    /// @brief Sets the window's check status (RADIOBUTTON and CHECKBOX).
    /// @param child Child window to check.
    /// @param check Status.
    void buttonSetCheck(Child *child, bool check);

#ifdef __cplusplus
}
#endif
