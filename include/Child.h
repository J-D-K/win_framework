#pragma once
#include "Window_external.h"

// This file contains functions that apply to all Child window structs.
#ifdef __cplusplus
extern "C"
{
#endif

    /// @brief Sets the WM_COMMAND function for the child window passed.
    /// @param child Child window to set the command function for.
    /// @param eventFunction Function that is executed when WM_COMMAND is received from the child.
    /// @param data Data to pass to the EventFunction.
    void childSetCommandFunction(Child *child, EventFunction commandFunction, void *commandData);

    /// @brief Sets the WM_NOTIFY function for the child window passed.
    /// @param child Child window to the the notify function for.
    /// @param eventFunction Function that is executed when the WM_NOTIFY command is received from the child.
    /// @param data Data to pass to the notify function.
    void childSetNotifyFunction(Child *child, EventFunction notifyFunction, void *notifyData);

    /// @brief Initializes and sets the child's functions to NULL.
    /// @param child Child to initialize functions for.
    void childInitFunctionsDefault(Child *child);

#ifdef __cplusplus
}
#endif
