#include "vector.h"

void vector_init(vector* vec) {
    if (vec == NULL) return;
    vec->SIZE = 0;
    memset(vec->array, 0, sizeof(vec->array));
}

void vector_pushback(vector* vec, int32_t val) {
    if (vec == NULL || vec->SIZE >= 256) return;
    vec->array[vec->SIZE++] = val;
}

vector initVector(void) {
    vector vec;
    vec.SIZE = 0;
    memset(vec.array, 0, sizeof(vec.array));
    return vec;
}

void pushbackVector(vector* vec, int32_t val) {
    if (vec == NULL) return;
    if (vec->SIZE < 256) {
        vec->array[vec->SIZE++] = val;
    } else {
        fprintf(stderr, "pushbackVector: vector is full\n");
    }
}

void popbackVector(vector* vec) {
    if (vec == NULL) return;
    if (vec->SIZE > 0) {
        vec->SIZE--;
    }
}

void vector_popback(vector* vec) {
    if (vec == NULL || vec->SIZE == 0) return;
    vec->SIZE--;
}

void clearVector(vector* vec) {
    if (vec == NULL) return;
    vec->SIZE = 0;
    memset(vec->array, 0, sizeof(vec->array));
}

void eraseVector(vector* vec, int index) {
    if (vec == NULL) return;
    if (index < 0 || (uint32_t)index >= vec->SIZE) {
        fprintf(stderr, "eraseVector: invalid index %d\n", index);
        return;
    }
    for (uint32_t i = index; i < vec->SIZE - 1; ++i) {
        vec->array[i] = vec->array[i + 1];
    }
    vec->SIZE--;
}

int32_t atVector(vector* vec, int index) {
    if (vec == NULL) return 0;
    if (index < 0 || (uint32_t)index >= vec->SIZE) {
        fprintf(stderr, "atVector: invalid index %d\n", index);
        return 0;
    }
    return vec->array[index];
}

void setVector(vector* vec, int index, int32_t val) {
    if (vec == NULL) return;
    if (index < 0 || (uint32_t)index >= vec->SIZE) {
        fprintf(stderr, "setVector: invalid index %d\n", index);
        return;
    }
    vec->array[index] = val;
}

int findVector(vector* vec, int32_t val) {
    if (vec == NULL) return -1;
    for (uint32_t i = 0; i < vec->SIZE; ++i) {
        if (vec->array[i] == val) {
            return (int)i;
        }
    }
    return -1;
}

void insertVector(vector* vec, int index, int32_t val) {
    if (vec == NULL) return;
    if (vec->SIZE >= 256) {
        fprintf(stderr, "insertVector: vector is full\n");
        return;
    }
    if (index < 0) index = 0;
    if ((uint32_t)index > vec->SIZE) index = (int)vec->SIZE;

    for (uint32_t i = vec->SIZE; i > (uint32_t)index; --i) {
        vec->array[i] = vec->array[i - 1];
    }
    vec->array[index] = val;
    vec->SIZE++;
}

bool isEmptyVector(vector* vec) {
    if (vec == NULL) return true;
    return vec->SIZE == 0;
}

void resizeVector(vector* vec, uint32_t newSize) {
    if (vec == NULL) return;
    if (newSize > 256) newSize = 256;
    if (newSize > vec->SIZE) {
        for (uint32_t i = vec->SIZE; i < newSize; ++i) {
            vec->array[i] = 0;
        }
    }
    vec->SIZE = newSize;
}

void printVector(vector* vec) {
    if (vec == NULL) return;
    printf("[");
    for (uint32_t i = 0; i < vec->SIZE; ++i) {
        printf("%d", vec->array[i]);
        if (i < vec->SIZE - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}
