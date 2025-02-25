#pragma once
#include "Window_external.h"

// This header contains functions related to progress bars.
#ifdef __cplusplus
extern "C"
{
#endif

    /// @brief Adds a progres bar to the window.
    /// @param window Window to add the bar to.
    /// @param x X coordinate.
    /// @param y Y coordinate.
    /// @param width Width.
    /// @param height Height.
    /// @param style Progress bar style.
    /// @return Progress bar window.
    Child *windowAddProgressBar(Window *window, int x, int y, int width, int height, DWORD style);

    /// @brief Sets the bar color of the progress bar passed.
    /// @param window Progress bar to set the color of.
    /// @param color Color to set to.
    void progressBarSetBarColor(Child *child, COLORREF color);

    /// @brief Sets the background color of the progress bar passed.
    /// @param window Progress bar to set the color of.
    /// @param color Color to set to.
    void progressBarSetBackColor(Child *child, COLORREF color);

    /// @brief Advances the position of the progress bar.
    /// @param window Progress bar to advance.
    /// @param advance Amount to advance bar.
    void progressBarAdvance(Child *child, int advance);

    /// @brief Sets the position of the progress bar.
    /// @param window Progress bar to set the position of.
    /// @param position Position to set.
    void progressBarSetPosition(Child *child, int position);

    /// @brief
    /// @param window
    /// @param minimum
    /// @param maximum
    void progressBarSetRange(Child *child, int minimum, int maximum);

#ifdef __cplusplus
}
#endif
