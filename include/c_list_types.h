#ifndef C_LIST_TYPES_H
#define C_LIST_TYPES_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define C_LIST_NO_ERROR 0
#define C_LIST_ERROR 255

/*-----------------------------TYPE Parameterized List----------------------------*/
#define create_c_list_headers(NAME,TYPE)                                           \
typedef struct NAME {                                                              \
  size_t nLength;                                                                  \
  size_t nLength_max;                                                              \
  TYPE *pList;                                                                     \
} NAME;                                                                            \
                                                                                   \
uint8_t NAME##_init(NAME *x, size_t length);                                       \
NAME *NAME##_alloc(size_t length);                                                 \
uint8_t NAME##_resize(NAME *x, size_t new_length);                                 \
size_t NAME##_length(NAME *x);                                                     \
uint8_t NAME##_push(NAME *x, TYPE value);                                          \
TYPE NAME##_pop(NAME *x);                                                          \
TYPE NAME##_peek(NAME *x);                                                         \
uint8_t NAME##_update(NAME *x, size_t index, TYPE value);                          \
TYPE NAME##_at(NAME *x, size_t index);                                             \
uint8_t NAME##_copy(NAME *dst, NAME *src);                                         \
void NAME##_free(NAME *x, void (*free_element)(TYPE *value));                      \

/*-----------------------------TYPE Parameterized List----------------------------*/
#define create_c_list_type(NAME,TYPE)                                              \
                                                                                   \
uint8_t NAME##_init(NAME *x, size_t length) {                                      \
  length = length==0?1:length;                                                     \
  x->nLength = 0;                                                                  \
  x->nLength_max = length;                                                         \
  x->pList = (TYPE *)calloc(x->nLength_max, sizeof(TYPE));                         \
  if(x->pList == NULL) {                                                           \
    return C_LIST_ERROR;                                                           \
  }                                                                                \
  return C_LIST_NO_ERROR;                                                          \
}                                                                                  \
                                                                                   \
NAME *NAME##_alloc(size_t length) {                                                \
  NAME *x = malloc(sizeof(NAME));                                                  \
  if(x == NULL) return NULL;                                                       \
  if(NAME##_init(x, length) == C_LIST_NO_ERROR)                                    \
    return x;                                                                      \
  free(x);                                                                         \
  return NULL;                                                                     \
}                                                                                  \
                                                                                   \
uint8_t NAME##_resize(NAME *x, size_t new_length) {                                \
  if(new_length > x->nLength_max) {                                                \
    x->pList =                                                                     \
      (TYPE *)realloc((void *)x->pList, new_length * sizeof(TYPE));                \
    if(x->pList == NULL) {                                                         \
      return C_LIST_ERROR;                                                         \
    }                                                                              \
    memset((void *)(x->pList + x->nLength_max),                                    \
           0, new_length - x->nLength_max);                                        \
    x->nLength_max = new_length;                                                   \
  } else if(new_length < x->nLength_max) {                                         \
    x->pList =                                                                     \
      (TYPE *)realloc((void*)x->pList, new_length * sizeof(TYPE));                 \
    if(x->pList == NULL) {                                                         \
      return C_LIST_ERROR;                                                         \
    }                                                                              \
    x->nLength_max = new_length;                                                   \
    if(x->nLength > x->nLength_max) x->nLength = x->nLength_max;                   \
  }                                                                                \
  return C_LIST_NO_ERROR;                                                          \
}                                                                                  \
                                                                                   \
size_t NAME##_length(NAME *x) {                                                    \
  return x->nLength;                                                               \
}                                                                                  \
                                                                                   \
uint8_t NAME##_push(NAME *x, TYPE value) {                                         \
  if(x->nLength == (size_t) ~0) return C_LIST_ERROR;                               \
  if(x->nLength+1 > x->nLength_max) {                                              \
    if(NAME##_resize(x, (x->nLength+1)*2) != C_LIST_NO_ERROR) return C_LIST_ERROR; \
  }                                                                                \
  x->pList[x->nLength++] = value;                                                  \
  return C_LIST_NO_ERROR;                                                          \
}                                                                                  \
                                                                                   \
TYPE NAME##_pop(NAME *x) {                                                         \
  TYPE ret = x->pList[--x->nLength];                                               \
  return ret;                                                                      \
}                                                                                  \
                                                                                   \
TYPE NAME##_peek(NAME *x) {                                                        \
  return x->pList[x->nLength-1];                                                   \
}                                                                                  \
                                                                                   \
uint8_t NAME##_update(NAME *x, size_t index, TYPE value) {                         \
  if(index >= x->nLength_max) {                                                    \
    if(NAME##_resize(x, index+1) != C_LIST_NO_ERROR) return C_LIST_ERROR;          \
  }                                                                                \
  x->pList[index] = value;                                                         \
  if(x->nLength <= index) x->nLength = index+1;                                    \
  return C_LIST_NO_ERROR;                                                          \
}                                                                                  \
                                                                                   \
TYPE NAME##_at(NAME *x, size_t index) {                                            \
  return x->pList[index];                                                          \
}                                                                                  \
                                                                                   \
uint8_t NAME##_copy(NAME *dst, NAME *src) {                                        \
  if(NAME##_resize(dst, src->nLength) != C_LIST_NO_ERROR) return C_LIST_ERROR;     \
  dst->nLength = src->nLength;                                                     \
  memcpy(dst->pList, src->pList, dst->nLength * sizeof(TYPE));                     \
  return C_LIST_NO_ERROR;                                                          \
}                                                                                  \
                                                                                   \
void NAME##_free(NAME *x, void (*free_element)(TYPE *value)) {                     \
  if(x->pList == NULL) return;                                                     \
  if(free_element) {                                                               \
    while(x->nLength) free_element(&x->pList[--x->nLength]);                       \
  }                                                                                \
  free((void *)x->pList);                                                          \
  x->pList = NULL;                                                                 \
  x->nLength = 0;                                                                  \
  x->nLength_max = 0;                                                              \
}                                                                                  \

#endif
