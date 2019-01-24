#ifndef C_QUEUE_TYPES_H
#define C_QUEUE_TYPES_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define C_QUEUE_NO_ERROR 0
#define C_QUEUE_ERROR 255

/*-----------------------------TYPE Parameterized Queue-------------------------*/
#define create_c_queue_headers(NAME,TYPE)                                        \
typedef struct NAME {                                                            \
  size_t head;                                                                   \
  size_t tail;                                                                   \
  size_t nLength;                                                                \
  size_t nLength_max;                                                            \
  TYPE *pList;                                                                   \
} NAME;                                                                          \
                                                                                 \
uint8_t NAME##_init(NAME *x, size_t length);                                     \
NAME *NAME##_alloc(size_t length);                                               \
uint8_t NAME##_resize(NAME *x, size_t new_length);                               \
size_t NAME##_length(NAME *x);                                                   \
TYPE NAME##_peek(NAME *x);                                                       \
uint8_t NAME##_empty(NAME *x);                                                   \
uint8_t NAME##_enqueue(NAME *x, TYPE value);                                     \
TYPE NAME##_dequeue(NAME *x);                                                    \
uint8_t NAME##_update(NAME *x, size_t index, TYPE value);                        \
TYPE NAME##_at(NAME *x, size_t index);                                           \
uint8_t NAME##_copy(NAME *dst, NAME *src);                                       \
void NAME##_clear(NAME *x);                                                      \
void NAME##_free(NAME *x, void (*free_element)(TYPE *value));                    \

/*-----------------------------TYPE Parameterized Queue-------------------------*/
#define create_c_queue_type(NAME,TYPE)                                           \
uint8_t NAME##_init(NAME *x, size_t length) {                                    \
  x->head = 0;                                                                   \
  x->tail = 0;                                                                   \
  length = length==0?1:length;                                                   \
  x->nLength = 0;                                                                \
  x->nLength_max = length;                                                       \
  x->pList = (TYPE *)calloc(x->nLength_max, sizeof(TYPE));                       \
  if(x->pList == NULL) {                                                         \
    return C_QUEUE_ERROR;                                                        \
  }                                                                              \
  return C_QUEUE_NO_ERROR;                                                       \
}                                                                                \
                                                                                 \
NAME *NAME##_alloc(size_t length) {                                              \
  NAME *x = malloc(sizeof(NAME));                                                \
  if(x == NULL) return NULL;                                                     \
  if(NAME##_init(x, length) == C_QUEUE_NO_ERROR)                                 \
    return x;                                                                    \
  free(x);                                                                       \
  return NULL;                                                                   \
}                                                                                \
                                                                                 \
uint8_t NAME##_resize(NAME *x, size_t new_length) {                              \
  assert(new_length >= x->nLength_max);                                          \
  x->pList =                                                                     \
    (TYPE *)realloc((void *)x->pList, new_length * sizeof(TYPE));                \
  if(x->pList == NULL) {                                                         \
    return C_QUEUE_ERROR;                                                        \
  }                                                                              \
  if(x->tail < x->head) {                                                        \
    size_t i;                                                                    \
    for(i = 0; i < x->tail; i++) {                                               \
      x->pList[(x->nLength_max + i) % new_length] = x->pList[i];                 \
    }                                                                            \
    x->tail = (x->nLength_max + x->tail) % new_length;                           \
  }                                                                              \
                                                                                 \
  x->nLength_max = new_length;                                                   \
                                                                                 \
  return C_QUEUE_NO_ERROR;                                                       \
}                                                                                \
                                                                                 \
size_t NAME##_length(NAME *x) {                                                  \
  return x->nLength;                                                             \
}                                                                                \
                                                                                 \
TYPE NAME##_peek(NAME *x) {                                                      \
  return x->pList[x->head];                                                      \
}                                                                                \
                                                                                 \
uint8_t NAME##_empty(NAME *x) {                                                  \
  return x->nLength == 0;                                                        \
}                                                                                \
                                                                                 \
uint8_t NAME##_enqueue(NAME *x, TYPE value) {                                    \
  if(x->nLength  == x->nLength_max) {                                            \
    uint8_t ret = NAME##_resize(x, x->nLength_max*2);                            \
    if(ret != C_QUEUE_NO_ERROR) return ret;                                      \
  }                                                                              \
                                                                                 \
  if(x->nLength == 0) {                                                          \
    x->pList[0] = value;                                                         \
    x->head = x->tail = 0;                                                       \
  } else {                                                                       \
    x->tail++; x->tail %= x->nLength_max;                                        \
    x->pList[x->tail] = value;                                                   \
  }                                                                              \
  x->nLength++;                                                                  \
                                                                                 \
  return C_QUEUE_NO_ERROR;                                                       \
}                                                                                \
                                                                                 \
TYPE NAME##_dequeue(NAME *x) {                                                   \
  if(x->nLength == 0) return 0;                                                  \
  TYPE ret = x->pList[x->head];                                                  \
  x->head++; x->head %= x->nLength_max;                                          \
  x->nLength--;                                                                  \
  return ret;                                                                    \
}                                                                                \
                                                                                 \
uint8_t NAME##_update(NAME *x, size_t index, TYPE value) {                       \
  if(index >= x->nLength_max) {                                                  \
    if(NAME##_resize(x, index+1) != C_QUEUE_NO_ERROR) return C_QUEUE_ERROR;      \
  }                                                                              \
  x->pList[index] = value;                                                       \
  return C_QUEUE_NO_ERROR;                                                       \
}                                                                                \
                                                                                 \
TYPE NAME##_at(NAME *x, size_t index) {                                          \
  return x->pList[index];                                                        \
}                                                                                \
                                                                                 \
uint8_t NAME##_copy(NAME *dst, NAME *src) {                                      \
  if(NAME##_resize(dst, src->nLength) != C_QUEUE_NO_ERROR) return C_QUEUE_ERROR; \
  dst->head = src->head;                                                         \
  dst->tail = src->tail;                                                         \
  dst->nLength = src->nLength;                                                   \
  memcpy(dst->pList, src->pList, dst->nLength * sizeof(TYPE));                   \
  return C_QUEUE_NO_ERROR;                                                       \
}                                                                                \
                                                                                 \
void NAME##_clear(NAME *x) {                                                     \
  x->head = 0;                                                                   \
  x->tail = 0;                                                                   \
  x->nLength = 0;                                                                \
}                                                                                \
                                                                                 \
void NAME##_free(NAME *x, void (*free_element)(TYPE *value)) {                   \
  if(x->pList == NULL) return;                                                   \
  if(free_element) {                                                             \
    while(x->nLength) free_element(&x->pList[--x->nLength]);                     \
  }                                                                              \
  free((void *)x->pList);                                                        \
  x->pList = NULL;                                                               \
  x->head = 0;                                                                   \
  x->tail = 0;                                                                   \
  x->nLength = 0;                                                                \
  x->nLength_max = 0;                                                            \
}                                                                                \

#endif
