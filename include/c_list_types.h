#ifndef LIST_TYPES_H
#define LIST_TYPES_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <stdio.h>

#define NO_ERROR 0
#define MEM_ERR 255

/*----------------------TYPE Parameterized List----------------------------*/
#define create_c_list_headers(NAME,TYPE)                                   \
typedef struct NAME {                                                      \
  size_t nLength;                                                          \
  size_t nLength_max;                                                      \
  TYPE *pList;                                                             \
  size_t nResizeAmt;                                                       \
} NAME;                                                                    \
                                                                           \
uint8_t NAME##_init(NAME *x, size_t length, size_t resize_amt);            \
NAME *NAME##_alloc(size_t length, size_t resize_amt);                      \
uint8_t NAME##_resize(NAME *x, size_t new_length);			   \
size_t NAME##_length(NAME *x);                                             \
uint8_t NAME##_push(NAME *x, TYPE value);                                  \
TYPE NAME##_pop(NAME *x);                                                  \
TYPE NAME##_peek(NAME *x);                                                 \
uint8_t NAME##_update(NAME *x, size_t index, TYPE value);                  \
TYPE NAME##_at(NAME *x, size_t index);                                     \
uint8_t NAME##_copy(NAME *dst, NAME *src);                                 \
void NAME##_free(NAME *x, void (*free_element)(TYPE value));               \

/*---------------------------------------TYPE Parameterized List-----------*/
#define create_c_list_type(NAME,TYPE)                                      \
                                                                           \
uint8_t NAME##_init(NAME *x, size_t length, size_t resize_amt) {           \
  length = length==0?1:length;                                             \
  resize_amt = resize_amt == 0?1:resize_amt;                               \
  x->nLength = 0;                                                          \
  x->nLength_max = length;                                                 \
  x->pList = (TYPE *)malloc(x->nLength_max * sizeof(TYPE));                \
  if(x->pList == NULL) {                                                   \
    return MEM_ERR;                                                        \
  }                                                                        \
  x->nResizeAmt = resize_amt;                                              \
  return NO_ERROR;                                                         \
}                                                                          \
                                                                           \
NAME *NAME##_alloc(size_t length, size_t resize_amt) {                     \
  NAME *x = malloc(sizeof(NAME));                                          \
  if(x == NULL) return NULL;                                               \
  if(NAME##_init(x, length, resize_amt) == NO_ERROR)                       \
    return x;                                                              \
  free(x);                                                                 \
  return NULL;                                                             \
}                                                                          \
                                                                           \
uint8_t NAME##_resize(NAME *x, size_t new_length) {                        \
  if(new_length > x->nLength_max) {                                        \
    size_t delta = new_length - x->nLength_max;                            \
    if(delta < x->nResizeAmt) new_length = x->nLength_max + x->nResizeAmt; \
    x->nLength_max = new_length;                                           \
    x->pList =                                                             \
      (TYPE *)realloc((void*)x->pList, x->nLength_max * sizeof(TYPE));     \
    if(x->pList == NULL) {                                                 \
      return MEM_ERR;                                                      \
    }                                                                      \
  }                                                                        \
  return NO_ERROR;                                                         \
}                                                                          \
                                                                           \
size_t NAME##_length(NAME *x) {                                            \
  return x->nLength;                                                       \
}                                                                          \
                                                                           \
uint8_t NAME##_push(NAME *x, TYPE value) {                                 \
  if(x->nLength == (size_t) -1) return MEM_ERR;                            \
  if(NAME##_resize(x, x->nLength+1) != NO_ERROR) return MEM_ERR;           \
  x->pList[x->nLength++] = value;                                          \
  return NO_ERROR;                                                         \
}                                                                          \
                                                                           \
TYPE NAME##_pop(NAME *x) {                                                 \
  if(x->nLength == 0) return 0;                                            \
  TYPE ret = x->pList[--x->nLength];                                       \
  return ret;                                                              \
}                                                                          \
                                                                           \
TYPE NAME##_peek(NAME *x) {                                                \
  if(x->nLength == 0) return 0;                                            \
  return x->pList[x->nLength-1];                                           \
}                                                                          \
                                                                           \
uint8_t NAME##_update(NAME *x, size_t index, TYPE value) {                 \
  if(NAME##_resize(x, index+1) != NO_ERROR) return MEM_ERR;                \
  x->pList[index] = value;                                                 \
  if(x->nLength <= index) x->nLength = index+1;                            \
  return NO_ERROR;                                                         \
}                                                                          \
                                                                           \
TYPE NAME##_at(NAME *x, size_t index) {                                    \
  assert(x->nLength > index);                                              \
  if(x->nLength <= index) return 0;                                        \
  return x->pList[index];                                                  \
}                                                                          \
                                                                           \
uint8_t NAME##_copy(NAME *dst, NAME *src) {                                \
  if(NAME##_resize(dst, src->nLength) != NO_ERROR) return MEM_ERR;	   \
  dst->nLength = src->nLength;                                             \
  memcpy(dst->pList, src->pList, dst->nLength * sizeof(TYPE));             \
  return NO_ERROR;                                                         \
}                                                                          \
                                                                           \
void NAME##_free(NAME *x, void (*free_element)(TYPE value)) {              \
  if(x->pList == NULL) return;                                             \
  if(free_element) {                                                       \
    while(x->nLength) {                                                    \
      if(x->pList[--x->nLength]) {                                         \
        free_element(x->pList[x->nLength]);                                \
      }                                                                    \
    }                                                                      \
  }                                                                        \
  free((void *)x->pList);                                                  \
  x->pList = NULL;                                                         \
  x->nLength = 0;                                                          \
  x->nLength_max = 0;                                                      \
  free(x);                                                                 \
}                                                                          \

#endif
