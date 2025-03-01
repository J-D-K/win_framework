#pragma once
#include "Menu_external.h"

// End-user facing functions for dealing with menus.
// windowAddMenuEvent should be used to add functionality to menus based upon the return value of menuAddMenu.
#ifdef __cplusplus
extern "C"
{
#endif
    /// @brief Creates a new menu.
    /// @return Pointer to new menu. NULL on failure.
    Menu *menuCreate(void);

    /// @brief Destroys and frees the memory used by a menu.
    /// @param menu Menu to destroy.
    void menuDestroy(Menu *menu);

    /// @brief Adds a new option to the menu.
    /// @param menu Menu to add menu to.
    /// @param menuText Text of menu.
    /// @param flags
    /// @return The index of the menu/option added. -1 on failure.
    int menuAddMenu(Menu *menu, char *menuText, const Menu *subMenu);

    /// @brief Adds a divider to the menu.
    /// @param menu Menu to add divider to.
    void menuAddSeparator(Menu *menu);

#ifdef __cplusplus
}
#endif
