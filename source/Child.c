#include "Child.h"

#define __WINDOW_INTERNAL__
#include "Window_internal.h"

void childSetCommandFunction(Child *child, EventFunction commandFunction, void *commandData)
{
    child->commandFunction = commandFunction;
    child->commandData = commandData;
}

void childSetNotifyFunction(Child *child, EventFunction notifyFunction, void *notifyData)
{
    child->notifyFunction = notifyFunction;
    child->notifyData = notifyData;
}

void childInitFunctionsDefault(Child *child)
{
    child->commandFunction = NULL;
    child->commandData = NULL;

    child->notifyFunction = NULL;
    child->notifyData = NULL;
}
