#pragma once
#include <stddef.h>

/// @brief Function pointer for freeing elements.
typedef void (*ElementFreeFunction)(void *);

/// @brief Dynamic array struct.
typedef struct DynamicArray DynamicArray;

/// @brief Creates a new dynamic array.
/// @param elementSize Size of the elements in the array.
/// @param freeFunction Function to call to free elements in array. Pass NULL if this is not needed.
/// @return New dynamic array on success. NULL on failure to allocate.
DynamicArray *dynamicArrayCreate(size_t elementSize, ElementFreeFunction freeFunction);

/// @brief Frees all elements in the array and the array itself. Afterwards, the pointer will become NULL.
/// @param array
void dynamicArrayDestroy(DynamicArray *array);

/// @brief Returns the current size of the array.
/// @param array Array to get the size of.
/// @return Number of elements currently in the array.
size_t dynamicArrayGetSize(DynamicArray *array);

/// @brief Allocates a new element in the array.
/// @param array Array to allocate new with.
/// @return Pointer to new element. Needs to be cast to type.
void *dynamicArrayNew(DynamicArray *array);

/// @brief Inserts (memcpy's) element passed to index.
/// @param array Array to insert element to.
/// @param index Place to inset element.
/// @param element Pointer to element to insert.
/// @return Pointer to element.
void *dynamicArrayInsert(DynamicArray *array, int index, void *element);

/// @brief Erases the element at index.
/// @param array Array to erase from.
/// @param index Index of element to erase.
void dynamicArrayErase(DynamicArray *array, int index);

/// @brief Erases a range of elements from the array.
/// @param array The array to erase from.
/// @param begin The first element to erase.
/// @param end The final element to erase.
void dynamicArrayEraseRange(DynamicArray *array, int begin, int end);

/// @brief Gets the element at index.
/// @param array Array to get the element from.
/// @param index Index to get.
/// @return Pointer to the element if valid. NULL if not.
void *dynamicArrayGet(DynamicArray *array, int index);
