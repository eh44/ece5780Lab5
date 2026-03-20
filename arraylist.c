/**
 * @file    arraylist.c
 * @brief   Implementation of dynamic ArrayList functions for C.
 *
 * This module implements creation, resizing, and manipulation routines for
 * an ArrayList capable of holding homogeneous elements of type char, int,
 * float, or double. It provides:
 *   - create_arraylist(): allocate and initialize a new list
 *   - arraylist_add():       append an element to the end
 *   - arraylist_insert():    insert at an arbitrary index
 *   - arraylist_remove():    remove an element at a given index
 *   - delete_arraylist():    free all resources
 *
 * @author  Ayoub EZZAOUYAH
 * @date    2025-04-25
 * @version 1.0
 */

#include "arraylist.h"

ArrayList *create_arraylist(DataType type)
{
    ArrayList *list = (ArrayList *)malloc(sizeof(ArrayList));
    int initial_capacity = 5;
    if (list != NULL)
    {
        switch (type)
        {
        case TYPE_CHAR:
            list->data = malloc(initial_capacity * sizeof(char));
            break;
        case TYPE_INT:
            list->data = malloc(initial_capacity * sizeof(int));
            break;
        case TYPE_FLOAT:
            list->data = malloc(initial_capacity * sizeof(float));
            break;
        case TYPE_DOUBLE:
            list->data = malloc(initial_capacity * sizeof(double));
            break;
        default:
            free(list);
        }
        if (list->data == NULL)
        {
            free(list);
            return NULL;
        }
        list->capacity = initial_capacity;
        list->length = 0;
        list->type = type;
        return list;
    }
    else
        return NULL;
}
void arraylist_add(ArrayList *list, void *element)
{
    int length = list->length;
    DataType type = list->type;

    if (length >= list->capacity)
    {
        int new_capacity = list->capacity * 2;
        void *temp = realloc(list->data, new_capacity * (type == TYPE_CHAR ? sizeof(char) : (type == TYPE_INT ? sizeof(int) : (type == TYPE_FLOAT ? sizeof(float) : sizeof(double)))));
        if (temp == NULL)
        {
            return;
        }
        list->data = temp;
        list->capacity = new_capacity;
    }

    switch (type)
    {
    case TYPE_CHAR:
        ((char *)list->data)[length] = *(char *)element;
        break;
    case TYPE_INT:
        ((int *)list->data)[length] = *(int *)element;
        break;
    case TYPE_FLOAT:
        ((float *)list->data)[length] = *(float *)element;
        break;
    case TYPE_DOUBLE:
        ((double *)list->data)[length] = *(double *)element;
        break;
    }
    list->length++;
}

void arraylist_insert(ArrayList *list, int index, void *value)
{
    if (index < 0 || index >= list->length)
    {
        return;
    }
    if (list->length >= list->capacity)
    {
        DataType type;
        int new_capacity = list->capacity * 2;
        void *temp = realloc(list->data, new_capacity * (type == TYPE_CHAR ? sizeof(char) : (type == TYPE_INT ? sizeof(int) : (type == TYPE_FLOAT ? sizeof(float) : sizeof(double)))));
        if (temp == NULL)
        {
            return;
        }
        list->data = temp;
        list->capacity = new_capacity;
    }
    for (int i = index + 1; i < list->length; i++)
    {
        switch (list->type)
        {
        case TYPE_CHAR:
            ((char *)list->data)[i] = ((char *)list->data)[i - 1];
            break;
        case TYPE_INT:
            ((int *)list->data)[i] = ((int *)list->data)[i - 1];
            break;
        case TYPE_FLOAT:
            ((float *)list->data)[i] = ((float *)list->data)[i - 1];
            break;
        case TYPE_DOUBLE:
            ((double *)list->data)[i] = ((double *)list->data)[i - 1];
            break;
        default:
            return;
        }
    }
    switch (list->type)
    {
    case TYPE_CHAR:
        ((char *)list->data)[index] = *(char *)value;
        break;
    case TYPE_INT:
        ((int *)list->data)[index] = *(int *)value;
        break;
    case TYPE_FLOAT:
        ((float *)list->data)[index] = *(float *)value;
        break;
    case TYPE_DOUBLE:
        ((double *)list->data)[index] = *(double *)value;
        break;
    }
    list->length++;
}

void arraylist_remove(ArrayList *list, int index)
{
    if (index < 0 || index >= list->length)
    {
        return;
    }
    int i;
    for (i = index + 1; i < list->length; i++)
    {
        switch (list->type)
        {
        case TYPE_CHAR:
            ((char *)list->data)[i - 1] = ((char *)list->data)[i];
            break;
        case TYPE_INT:
            ((int *)list->data)[i - 1] = ((int *)list->data)[i];
            break;
        case TYPE_FLOAT:
            ((float *)list->data)[i - 1] = ((float *)list->data)[i];
            break;
        case TYPE_DOUBLE:
            ((double *)list->data)[i - 1] = ((double *)list->data)[i];
            break;
        }
    }
    list->length--;
    if (list->length < list->capacity / 2)
    {
        DataType type = list->type;
        int new_capacity = list->capacity / 2;
        void *temp = realloc(list->data, new_capacity * (type == TYPE_CHAR ? sizeof(char) : (type == TYPE_INT ? sizeof(int) : (type == TYPE_FLOAT ? sizeof(float) : sizeof(double)))));
        if (temp == NULL)
        {
            return;
        }
        list->data = temp;
        list->capacity = new_capacity;
    }
}
void *arraylist_get(ArrayList *list, int index)
{
    // 1. Check for out-of-bounds access
    if (index < 0 || index >= list->length)
    {
        return NULL; 
    }

    // 2. Return the address of the element at the requested index
    switch (list->type)
    {
    case TYPE_CHAR:
        return &((char *)list->data)[index];
    case TYPE_INT:
        return &((int *)list->data)[index];
    case TYPE_FLOAT:
        return &((float *)list->data)[index];
    case TYPE_DOUBLE:
        return &((double *)list->data)[index];
    default:
        return NULL;
    }
}
void delete_arraylist(ArrayList *list)
{
    free(list->data);
    free(list);
    return;
}