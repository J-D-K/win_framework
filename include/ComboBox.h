#pragma once
#include "Window_external.h"

// This header contains all functions related to combox boxes
#ifdef __cplusplus
extern "C"
{
#endif

    /// @brief Adds a combobox to the window.
    /// @param window Window to add combo box to.
    /// @param x X coordinate
    /// @param y Y coordinate
    /// @param width Width of box.
    /// @param height Height of box. AUTO_SIZE is not valid for this.
    /// @param eventFunction Function executed on event.
    /// @param data Data to pass to eventFunction.
    /// @return Window pointer on success. NULL on failure.1
    Child *windowAddComboxBox(Window *window,
                              int x,
                              int y,
                              int width,
                              int height,
                              DWORD style,
                              EventFunction eventFunction,
                              void *data);

    /// @brief Adds a string to a combobox.
    /// @param window Window struct with a handle to a combo box.
    /// @param string String to add to the ComboBox
    /// @return True on success. False on failure.
    bool comboBoxAddString(Child *child, const char *string);

    /// @brief Deletes the string at index in combo box.
    /// @param window Window struct with a handle to a combo box.
    /// @param index Index of string to delete.
    /// @return True on success. False on failure.
    bool comboBoxDeleteString(Child *child, int index);

    /// @brief Gets the count of strings in the combo box.
    /// @param window Combo box to get count of.
    /// @return Number of strings in box. CB_ERR if error occurs.
    int comboBoxGetCount(Child *child);
#ifdef __cplusplus
}
#endif
