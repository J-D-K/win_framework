#include "Menu.h"
#include <malloc.h>
#include <string.h>

#define __WINDOW_INTERNAL__
#include "Menu_internal.h"

Menu *menuCreate(void)
{
    Menu *menu = malloc(sizeof(Menu));
    if (!menu)
    {
        return NULL;
    }

    // Create a new menu.
    menu->menu = CreateMenu();
    // Set the position to 0?
    menu->position = 0;

    return menu;
}

void menuDestroy(Menu *menu)
{
    if (!menu)
    {
        return;
    }
    // Destroy the windows menu first (if possible). I hope Windows handles NULL pointers right.
    DestroyMenu(menu->menu);
    // Free the menu struct
    free(menu);
    // Set the pointer to NULL
    menu = NULL;
}

int menuAddMenu(Menu *menu, char *menuText, const Menu *subMenu)
{
    // This is the "master" menu identifier index thingy.
    static int menuIdentifier = 0;

    int position = menu->position++;

    MENUITEMINFOA itemInfo = {.cbSize = sizeof(MENUITEMINFOA),
                              .fMask = MIIM_STRING | MIIM_ID | (subMenu == NULL ? 0 : MIIM_SUBMENU),
                              .fType = MFT_STRING,
                              .fState = MFS_ENABLED,
                              .wID = menuIdentifier,
                              .hSubMenu = subMenu == NULL ? NULL : subMenu->menu,
                              .hbmpChecked = NULL,
                              .hbmpUnchecked = NULL,
                              .dwItemData = 0,
                              .dwTypeData = menuText,
                              .cch = strlen(menuText),
                              .hbmpItem = NULL};

    if (InsertMenuItemA(menu->menu, position, TRUE, &itemInfo) == FALSE)
    {
        return -1;
    }

    return menuIdentifier++;
}

void menuAddSeparator(Menu *menu)
{
    // Position.
    int position = menu->position++;

    InsertMenuA(menu->menu, position, MF_SEPARATOR, position, NULL);
}
