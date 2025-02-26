#pragma once
#include "Window_external.h"

/// @brief Gets an open filename.
/// @param window Window that owns the dialog.
/// @param title Title of the dialog.
/// @param filter File type filter string.
/// @param flags Flags to use for the dialog. Pass 0 to just use default/none.
/// @param buffer Buffer to write the filename to.
/// @param bufferSize Size of the buffer.
/// @return True on success. False on failure.
bool getOpenFilename(Window *window,
                     const char *title,
                     const char *filter,
                     DWORD flags,
                     char *buffer,
                     size_t bufferSize);

/// @brief Gets a save filename.
/// @param window Window that owns the dialog.
/// @param title Title of the dialog.
/// @param filter File type filter string.
/// @param buffer Buffer to write the filename to. Also the default, I think?
/// @param bufferSize Size of the buffer.
/// @return True on success. False on failure.
bool getSaveFilename(Window *window,
                     const char *title,
                     const char *filter,
                     DWORD flags,
                     char *buffer,
                     size_t bufferSize);
