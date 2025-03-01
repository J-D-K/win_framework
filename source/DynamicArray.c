#include "DynamicArray.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

// Actual complete struct.
struct DynamicArray
{
    // Pointer to the actual array.
    void **array;
    // Size of the elements being added.
    size_t elementSize;
    // Size/length of the array.
    int size;
    // Function used to free the memory in the array.
    ElementFreeFunction freeFunction;
};

// Declarations here. Defs later.
// Frees all elements in the array. This is needed by two functions in this file.
static void dynamicArrayFreeAll(DynamicArray *array);

DynamicArray *dynamicArrayCreate(size_t elementSize, ElementFreeFunction freeFunction)
{
    // This is the array we're returning.
    DynamicArray *returnArray = malloc(sizeof(DynamicArray));
    if (!returnArray)
    {
        // Return NULL.
        return NULL;
    }
    // Set array shit.
    returnArray->array = NULL;
    returnArray->size = 0;
    returnArray->elementSize = elementSize;
    returnArray->freeFunction = freeFunction;
    // Return it.
    return returnArray;
}

void dynamicArrayDestroy(DynamicArray *array)
{
    // Oops...
    if (!array)
    {
        // Abort mission.
        return;
    }

    // This will free all the elements.
    dynamicArrayFreeAll(array);

    // Free the array itself.
    free(array);
    // Set the array to NULL for segfaults.
    array = NULL;
}

int dynamicArrayGetSize(DynamicArray *array)
{
    return array->size;
}

void *dynamicArrayNew(DynamicArray *array)
{
    // Reallocate array to fit new element.
    array->array = realloc(array->array, sizeof(void *) * ++array->size);
    // Allocate the element.
    array->array[array->size - 1] = malloc(array->elementSize);
    // Return the pointer to it.
    return array->array[array->size - 1];
}

void *dynamicArrayInsert(DynamicArray *array, int index, void *element)
{
    if (index < 0 || index >= array->size)
    {
        return NULL;
    }
    // Realloc array to be able to insert new element.
    array->array = realloc(array->array, sizeof(void *) * ++array->size);

    // Shift everything in the array up 1 starting at index.
    for (size_t i = array->size - 1; i > index; i--)
    {
        array->array[i] = array->array[i - 1];
    }

    // Allocate the new element being inserted.
    array->array[index] = malloc(array->elementSize);

    // Memcpy
    memcpy(array->array[index], element, array->elementSize);
}

void dynamicArrayErase(DynamicArray *array, int index)
{
    if (index < 0 || index >= array->size)
    {
        return;
    }

    // Free the element at the index.
    if (array->freeFunction)
    {
        (*array->freeFunction)(array->array[index]);
    }
    else
    {
        free(array->array[index]);
    }

    // Loop through the array and collapse it.
    for (int i = index; i < (int)array->size; i++)
    {
        array->array[i] = array->array[i + 1];
    }

    // Realloc?
    array->array = realloc(array->array, sizeof(void *) * --array->size);
}

void dynamicArrayEraseRange(DynamicArray *array, int begin, int end)
{
    if ((begin < 0 || begin >= array->size) || (end < 0 || end >= array->size) || end <= begin)
    {
        return;
    }

    // Have to do this according to whether or not a function was set to free.
    if (array->freeFunction)
    {
        for (int i = begin; i <= end; i++)
        {
            (*array->freeFunction)(array->array[i]);
        }
    }
    else
    {
        for (int i = begin; i <= end; i++)
        {
            free(array->array[i]);
        }
    }
    // Collapse the array.
    for (int i = begin, j = end; i <= end; i++, j++)
    {
        array->array[i] = array->array[j];
    }
    // Realloc
    array->array = realloc(array->array, sizeof(void *) * (array->size -= end - begin));
}

void dynamicArrayClear(DynamicArray *array)
{
    // Free all of the elements.
    dynamicArrayFreeAll(array);

    // Free the array itself and make sure it's NULL.
    free(array->array);
    array->array = NULL;

    // Reset size to 0.
    array->size = 0;
}

void *dynamicArrayGet(DynamicArray *array, int index)
{
    if (index < 0 || index >= (int)array->size)
    {
        return NULL;
    }
    return array->array[index];
}

static void dynamicArrayFreeAll(DynamicArray *array)
{
    if (!array || !array->array)
    {
        // Just bail since there's nothing to do.
        return;
    }

    // Loop and free all the elements.
    if (array->freeFunction)
    {
        // Free elements using the passed function.
        for (int i = 0; i < array->size; i++)
        {
            (*array->freeFunction)(array->array[i]);
        }
    }
    else
    {
        // Just do it the old fashioned way.
        for (int i = 0; i < array->size; i++)
        {
            free(array->array[i]);
        }
    }
}
