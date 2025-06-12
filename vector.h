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

vector initVector(void);
void pushbackVector(vector *vec, int32_t val);
void popbackVector(vector *vec);
void clearVector(vector *vec);
void eraseVector(vector *vec, int index);
void vector_popback(vector *vec);

// 新增的功能
void vector_init(vector *vec);
void vector_pushback(vector *vec, int32_t val);
int32_t atVector(vector *vec, int index);
void setVector(vector *vec, int index, int32_t val);
int findVector(vector *vec, int32_t val);
void insertVector(vector *vec, int index, int32_t val);
bool isEmptyVector(vector *vec);
void resizeVector(vector *vec, uint32_t newSize);
void printVector(vector *vec);
void shuffle_deck(vector *vec);

void vector_destroy(vector *vec);

#endif // _VECTOR_H
