#include "display.h"
#include <windows.h>

int displayGetWidth(void)
{
    return GetSystemMetrics(SM_CXSCREEN);
}

int displayGetHeight(void)
{
    return GetSystemMetrics(SM_CYSCREEN);
}
