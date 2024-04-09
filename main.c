#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  void *data;
  size_t datasz;
  struct Node *next;
} Node;

typedef struct {
  Node *head;
} List;

#define LIST_CREATE() _list_create();
List _list_create(void) { return (List) { .head = NULL }; }

#define NODE_CREATE(T, V) ({ T _x = V; _node_create(&_x, sizeof(T)); });
Node *_node_create(void *data, size_t sz) {
  Node *nn = malloc(sizeof(Node));
  nn->datasz = sz;
  nn->data = malloc(sz);
  memcpy(nn->data, data, sz);
  return nn;
}

#define LIST_APPEND(LST, NN)                        \
  do {                                              \
    if ((LST)->head == NULL) (LST)->head = NN;      \
    else {                                          \
      Node *curr = (LST)->head;                     \
      while (curr->next != NULL) curr = curr->next; \
      curr->next = (NN);                            \
    }                                               \
  } while(0);                                       \

int main(void) {
  List lst = LIST_CREATE();
  Node *nn1 = NODE_CREATE(int, 1);

  LIST_APPEND(&lst, NODE_CREATE(int, 2));

  return 0;
}
