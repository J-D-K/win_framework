#pragma once
#include "Window_external.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /// @brief Adds a list box to the window.
    /// @param window Window to add listbox to.
    /// @param x X coordinate.
    /// @param y Y coordinate.
    /// @param width Width of list box.
    /// @param height Height of list box.
    /// @param style Listbox style.
    /// @param eventFunction Function that is executed when an event occurs.
    /// @param data Pointer that is passed to ^.
    /// @return
    Child *windowAddListBox(Window *window,
                            int x,
                            int y,
                            int width,
                            int height,
                            DWORD style,
                            EventFunction eventFunction,
                            void *data);

    /// @brief Adds a string to the listbox.
    /// @param child Listbox to add the string to.
    /// @param string String to add to the list box.
    bool listBoxAddString(Child *child, const char *string);

    /// @brief Gets the number of strings currently in the list box.
    /// @param child Listbox to get count for.
    /// @return Number of strings in the listbox.
    int listBoxGetCount(Child *child);

    /// @brief Gets the index of the currently selected item.
    /// @param child List box to get the selected index of.
    /// @return Selected index.
    int listBoxGetCurrentSelected(Child *child);

    /// @brief Gets the number of selected items for a multi-select list box.
    /// @param child List box to get count for.
    /// @return Number of currently selected items.
    int listBoxGetSelectedCount(Child *child);

    /// @brief Gets the currently selected items for a multi-select list box.
    /// @param child List box to get selected items from.
    /// @param selectedOut Array to store selected items.
    /// @param maxOut Array's length.
    /// @return Number of items read from the list box.
    int listBoxGetSelected(Child *child, int *selectedOut, int maxOut);

    /// @brief Gets the length of the text at index.
    /// @param child List box
    /// @param index
    /// @return
    int listBoxGetTextLength(Child *child, int index);

    /// @brief Gets the text of the item at index.
    /// @param child List box to get the text of.
    /// @param index Index of item to get the text of.
    /// @param buffer Buffer to write text to.
    /// @param bufferSize Size of buffer.
    /// @return True on success. False on failure.
    bool listBoxGetText(Child *child, int index, char *buffer, size_t bufferSize);

    /// @brief Deletes the string at index.
    /// @param child Listbox to delete a string from.
    /// @param index The index of the string to delete.
    /// @return True on success. False on error.
    bool listBoxDeleteString(Child *child, int index);

    /// @brief Shortcut function to clear the list box.
    /// @param child Listbox to clear.
    /// @return True on success. False on failure.
    bool listBoxClear(Child *child);

#ifdef __cplusplus
}
#endif
