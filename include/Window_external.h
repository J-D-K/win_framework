#pragma once
#include <stdbool.h>
#include <windows.h>

// This file contains definitions all headers and source files need.

#ifdef __cplusplus
extern "C"
{
#endif
    // Value to auto size controls.
    static const int AUTO_SIZE = -1;

    /// @brief Window struct.
    typedef struct Window Window;

    /// @brief Event function definition.
    typedef void (*EventFunction)(Window *window, WPARAM, void *);

#ifdef __cplusplus
}
#endif
