#pragma once
#include "Window_external.h"

// This header contains all functions related to edit controls.
#ifdef __cplusplus
extern "C"
{
#endif
    /// @brief Adds a text input to the window.
    /// @param window Window to add text input to.
    /// @param x X coordinate of input.
    /// @param y Y coordinate of input.
    /// @param width Width of the input box.
    /// @param background Color of the background of the input.
    /// @param style Style of EDIT control.
    /// @param eventFunction Function that is called for an event.
    /// @return Handle of the window/child on success. NULL on failure.
    Child *windowAddEdit(Window *window, int x, int y, int width, int height, DWORD style);

    /// @brief Gets the length of the text in the edit control.
    /// @param window Edit to get the length of.
    /// @return Length of the text.
    int editGetTextLength(Child *child);

    /// @brief Gets the character index of the the line at lineIndex.
    /// @param child Edit to get the index of.
    /// @param lineIndex Line to get the starting character index of.
    /// @return Index of the starting character of the line.
    int editGetLineIndex(Child *child, int lineIndex);

    /// @brief Gets the length if the line at index.
    /// @param child Edit to get the length of.
    /// @param index Index of the line to get the length of.
    /// @return Number of characters int the line.
    int editGetLineLength(Child *child, int index);

    /// @brief Gets the number of lines in a multi-line edit control.
    /// @param child Edit to get the count of.
    /// @return Number of lines in the edit.
    int editGetLineCount(Child *child);

    /// @brief Retrieves the text from a window.
    /// @param window Window of input.
    /// @param id ID of the input.
    /// @param buffer Buffer to write text to.
    /// @param bufferSize Size of the buffer to write text to.
    /// @return True on success. False on failure or buffer is too small.
    bool editGetText(Child *child, char *buffer, size_t bufferSize);

    /// @brief Gets the line at index from a multi-line edit control.
    /// @param child Edit control to get line from.
    /// @param index Index of line to get.
    /// @param buffer Buffer to write the line to.
    /// @param bufferSize Size of the buffer to write the line to.
    /// @return True on success. False on failure.
    bool editGetLine(Child *child, int index, char *buffer, int bufferSize);

    /// @brief Sets the text of a window.
    /// @param window Window owning text input.
    /// @param id The id of the input to set.
    /// @param text Text to set the input to.
    /// @return Handle of the window/child on success. NULL on failure.
    bool editSetText(Child *child, const char *text);

    /// @brief Appends text to the input with id.
    /// @param window Window owning the text input.
    /// @param id ID of the input to append to.
    /// @param text Text to append.
    /// @return Handle of the window/child on success. NULL on failure.
    void editAppendText(Child *child, const char *text);

#ifdef __cplusplus
}
#endif
