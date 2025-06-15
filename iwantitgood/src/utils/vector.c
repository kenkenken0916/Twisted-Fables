#include "vector.h"

vector initVector()
{
    vector vec;
    vec.SIZE = 0;
    for (int i = 0; i < 256; i++)
    {
        vec.array[i] = 0;
    }
    return vec;
}

void pushbackVector(vector *vec, int32_t val)
{
    if (vec && vec->SIZE < 256)
    {
        vec->array[vec->SIZE] = val;
        vec->SIZE++;
    }
}

void popbackVector(vector *vec)
{
    if (vec && vec->SIZE > 0)
    {
        vec->SIZE--;
        vec->array[vec->SIZE] = 0;
    }
}

void clearVector(vector *vec)
{
    if (vec)
    {
        vec->SIZE = 0;
        for (int i = 0; i < 256; i++)
        {
            vec->array[i] = 0;
        }
    }
}

void eraseVector(vector *vec, int index)
{
    if (vec && index >= 0 && index < (int)vec->SIZE)
    {
        // Shift all elements after index to the left
        for (int i = index; i < (int)vec->SIZE - 1; i++)
        {
            vec->array[i] = vec->array[i + 1];
        }
        vec->SIZE--;
        vec->array[vec->SIZE] = 0;
    }
}

int32_t deleteValueVector(vector *vec, int32_t val)
{
    if (!vec)
    {
        return 0;
    }

    int32_t deleted = 0;
    int writeIndex = 0;

    // Iterate through the vector, keeping only non-matching elements
    for (int readIndex = 0; readIndex < (int)vec->SIZE; readIndex++)
    {
        if (vec->array[readIndex] != val)
        {
            // Keep this element by moving it to writeIndex if needed
            if (writeIndex != readIndex)
            {
                vec->array[writeIndex] = vec->array[readIndex];
            }
            writeIndex++;
        }
        else
        {
            deleted++; // Count elements being deleted
        }
    }

    // Update size and clear unused elements
    vec->SIZE -= deleted;
    for (int i = vec->SIZE; i < vec->SIZE + deleted; i++)
    {
        vec->array[i] = 0;
    }

    return deleted;
}

int32_t getVector(const vector *vec, int index, int32_t *success)
{

    // Check for null pointer and valid index range
    if (!vec || index < 0 || index >= (int)vec->SIZE)
    {
        return 0; // Return 0 for invalid access
    }
    *success = vec->array[index]; // Mark as successful
    return 1;
}