#ifndef _VECTOR_H
#define _VECTOR_H
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct _vector
{
    int32_t array[256];
    uint32_t SIZE;
} vector;
vector initVector();
void pushbackVector(vector *vec, int32_t val);
void popbackVector(vector *vec);
void clearVector(vector *vec);
void eraseVector(vector *vec, int index);

/**
 * Delete all occurrences of a specific value from the vector
 * @param vec Pointer to the vector
 * @param val Value to delete from the vector
 * @return Number of elements deleted
 */
int32_t deleteValueVector(vector *vec, int32_t val);

/**
 * Get an element from the vector at the specified index
 * @param vec Pointer to the vector
 * @param index Index of the element to get (0-based)
 * @param success Pointer to a bool that will be set to true if successful, false if index out of bounds
 * @return The element at the specified index, or 0 if index is out of bounds
 */
int32_t getVector(const vector *vec, int index, int32_t *success);
#endif