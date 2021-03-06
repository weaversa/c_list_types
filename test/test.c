#include "test.h"

#include <stdio.h>

create_c_list_type(uint8_t_list, uint8_t)
create_c_list_type(uint8_t_plist, uint8_t *)

create_c_queue_type(uint8_t_queue, uint8_t)
create_c_queue_type(uint8_t_pqueue, uint8_t *)

void uint8_t_free(uint8_t **x) {
  free(*x);
}

int main (int argc, char *argv[]) {

  uint8_t_list *myList = uint8_t_list_alloc(0);

  for(uint32_t i = 0; i < 10000; i++) {
    uint8_t_list_push(myList, (uint8_t) i);
  }
  
  uint8_t_list_free(myList, NULL);

  
  uint8_t_plist *myPointerList = uint8_t_plist_alloc(0);

  for(uint32_t i = 0; i < 10; i++) {
    uint8_t *element = malloc(sizeof(uint8_t));
    *element = (uint8_t) i;
    uint8_t_plist_push(myPointerList, element);
  }
  uint8_t_plist_update(myPointerList, 40000, malloc(sizeof(uint8_t)));
  uint8_t_plist_update(myPointerList, 400000, malloc(sizeof(uint8_t))); 
  
  uint8_t_plist_free(myPointerList, uint8_t_free);

  
  uint8_t_queue *myQueue = uint8_t_queue_alloc(0);
  for(uint32_t i = 0; i < 1000000; i++) {
    uint8_t_queue_enqueue(myQueue, (uint8_t) i);
  }

  for(uint32_t i = 0; i < 1000000; i++) {
    uint8_t j = uint8_t_queue_dequeue(myQueue);
    assert(j == (uint8_t) i);
  }
  uint8_t_queue_free(myQueue, NULL);
  

  uint8_t_pqueue *mypQueue = uint8_t_pqueue_alloc(0);
  for(uint32_t i = 0; i < 1000000; i++) {
    uint8_t *element = malloc(sizeof(uint8_t));
    *element = (uint8_t) i;
    uint8_t_pqueue_enqueue(mypQueue, element);
  }

  for(uint32_t i = 0; i < 1000000; i++) {
    uint8_t *element = uint8_t_pqueue_dequeue(mypQueue);
    assert(*element == (uint8_t) i);
    free(element);
  }

  uint8_t_pqueue_free(mypQueue, NULL);
  
  return 0;
}
