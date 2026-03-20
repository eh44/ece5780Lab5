/**
 * @file    arraylist.h
 * @brief   A dynamic ArrayList implementation in C, providing easy creation,
 *          resizing, and manipulation of arrays storing elements of type char,
 *          int, float, or double.
 *
 * This module defines the ArrayList structure and a suite of functions to:
 *   - Initialize and destroy an ArrayList
 *   - Append, insert, remove, and retrieve elements
 *   - Automatically grow or shrink capacity as needed
 *   - Clear all elements or shrink capacity to match current size
 *
 * @author  Ayoub EZZAOUYAH
 * @date    2025-04-25
 * @version 1.0
 *
 * @copyright
 * (c) 2025 Ayoub EZZAOUYAH. All rights reserved.
 */

#ifndef _ARRAYLIST_H
#define _ARRAYLIST_H

#include <stdlib.h>

/**
 * @enum DataType
 * @brief Supported element types for the ArrayList.
 */
typedef enum
{
    TYPE_CHAR,  /**< char element (1 byte) */
    TYPE_INT,   /**< int element */
    TYPE_FLOAT, /**< float element */
    TYPE_DOUBLE /**< double element */
} DataType;

/**
 * @struct ArrayList
 * @brief Represents a resizable array of homogeneous elements.
 *
 * @var ArrayList::data
 *   Pointer to the element storage buffer.
 * @var ArrayList::capacity
 *   Total number of elements that can be stored before resizing.
 * @var ArrayList::length
 *   Current number of elements in the list.
 * @var ArrayList::type
 *   The DataType of elements in the list.
 */
typedef struct ArrayList
{
    void *data;
    int capacity;
    int length;
    DataType type;
} ArrayList;

/**
 * @brief   Create a new ArrayList capable of storing elements of the given type.
 * @param   type  The DataType of elements this list will hold.
 * @return  Pointer to a newly allocated ArrayList on success, NULL on failure.
 *
 * @details
 *   Allocates and initializes an ArrayList with an initial capacity of 5 elements.
 *   The underlying data buffer is allocated based on the chosen DataType size.
 */
ArrayList *create_arraylist(DataType type);

/**
 * @brief   Append an element to the end of the list.
 * @param   list      Pointer to the ArrayList.
 * @param   element   Pointer to the value to add. Must point to a variable of the list’s DataType.
 *
 * @details
 *   If the list is full, its capacity is doubled before adding the new element.
 */
void arraylist_add(ArrayList *list, void *element);

/**
 * @brief   Insert an element at a given index, shifting subsequent elements right.
 * @param   list    Pointer to the ArrayList.
 * @param   index   Zero-based position at which to insert the new element.
 * @param   value   Pointer to the value to insert. Must point to a variable of the list’s DataType.
 *
 * @details
 *   If index is out of range [0..length], the function returns without change.
 *   If the list is full, its capacity is doubled before insertion.
 */
void arraylist_insert(ArrayList *list, int index, void *value);

/**
 * @brief   Remove the element at the specified index, shifting following elements left.
 * @param   list   Pointer to the ArrayList.
 * @param   index  Zero-based position of the element to remove.
 *
 * @details
 *   If index is out of range [0..length-1], the function returns without change.
 *   After removal, if the number of elements falls below half the capacity,
 *   the capacity is halved to reclaim memory.
 */
void arraylist_remove(ArrayList *list, int index);

/**
 * @brief   Destroy the ArrayList and free all associated memory.
 * @param   list   Pointer to the ArrayList to delete.
 *
 * @details
 *   Frees the internal data buffer and then frees the ArrayList struct itself.
 */
void *arraylist_get(ArrayList *list, int index);
void delete_arraylist(ArrayList *list);

#endif // _ARRAYLIST_H
