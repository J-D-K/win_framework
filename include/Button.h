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
    Window *windowAddButton(Window *window,
                            int x,
                            int y,
                            int width,
                            int height,
                            const char *buttonText,
                            DWORD style,
                            EventFunction eventFunction,
                            void *data);

    /// @brief Returns if the window passed is check (RADIOBUTTON and CHECKBOX)
    /// @param window Window to check.
    /// @return True if checked. False if not.
    bool buttonGetCheck(Window *window);

    /// @brief Sets the window's check status (RADIOBUTTON and CHECKBOX).
    /// @param window Window to set.
    /// @param check Status.
    void buttonSetCheck(Window *window, bool check);

#ifdef __cplusplus
}
#endif
