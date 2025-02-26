#include "filename.h"
#include <windows.h>

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

bool getOpenFilename(Window *window,
                     const char *title,
                     const char *filter,
                     DWORD flags,
                     char *buffer,
                     size_t bufferSize)
{
    // Screw filling this out the prettier way. It's too big.
    OPENFILENAMEA openFilename = {0};
    openFilename.lStructSize = sizeof(OPENFILENAMEA);
    openFilename.hwndOwner = window->handle;
    openFilename.hInstance = window->appHandle;
    openFilename.lpstrFilter = filter;
    openFilename.nFilterIndex = 0;
    openFilename.lpstrFile = buffer;
    openFilename.nMaxFile = bufferSize;
    openFilename.lpstrTitle = title;
    openFilename.Flags = flags;

    if (GetOpenFileNameA(&openFilename) == FALSE)
    {
        return false;
    }
    return true;
}

bool getSaveFilename(Window *window,
                     const char *title,
                     const char *filter,
                     DWORD flags,
                     char *buffer,
                     size_t bufferSize)
{
    // Same as above.
    OPENFILENAMEA saveFilename = {0};
    saveFilename.lStructSize = sizeof(OPENFILENAMEA);
    saveFilename.hwndOwner = window->handle;
    saveFilename.hInstance = window->appHandle;
    saveFilename.lpstrFilter = filter;
    saveFilename.nFilterIndex = 0;
    saveFilename.lpstrFile = buffer;
    saveFilename.nMaxFile = bufferSize;
    saveFilename.lpstrTitle = title;
    saveFilename.Flags = flags;

    if (GetSaveFileNameA(&saveFilename) == FALSE)
    {
        return false;
    }
    return true;
}
