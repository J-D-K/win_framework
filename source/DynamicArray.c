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

    // If the free function is defined, use that.
    if (array->freeFunction)
    {
        for (size_t i = 0; i < array->size; i++)
        {
            (*array->freeFunction)(array->array[i]);
        }
    }
    else
    {
        // Just free the elements in this case.
        for (size_t i = 0; i < array->size; i++)
        {
            free(array->array[i]);
        }
    }
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

void *dynamicArrayGet(DynamicArray *array, int index)
{
    if (index < 0 || index >= (int)array->size)
    {
        return NULL;
    }
    return array->array[index];
}
