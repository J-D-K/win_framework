#pragma once
#include "Window_external.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /// @brief Adds a tab control to the window.
    /// @param window Window to add the tab control to.
    /// @param x X coordinate for the tab control.
    /// @param y Y coordinate for the tab control.
    /// @param width Width of the tab control.
    /// @param height Height of the tab control.
    /// @param style Style of the tab control.
    /// @param eventFunction Function executed when an even occurs.
    /// @param data Data passed to ^.
    /// @return Child window on success. NULL on failure.
    Child *windowAddTabControl(Window *window,
                               int x,
                               int y,
                               int width,
                               int height,
                               DWORD style,
                               EventFunction eventFunction,
                               void *data);

    /// @brief Adds a tab to a tab control.
    /// @param child Tab control to add the new tab to.
    /// @param index Index or place to add tab.
    /// @param tabTitle Name of the tab.
    /// @return Internal ID on success. -1 on failure.
    /// @note The window passed to this needs to be created with WS_CHILD.
    int tabControlAddTab(Child *child, int index, char *tabTitle);

#ifdef __cplusplus
}
#endif
