#ifndef LIST_H_
#define LIST_H_
#include <stdio.h>
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

List _list_create(void);
Node *_node_create(void *, size_t);
void _list_append(List *, Node *);

#define LIST_CREATE() _list_create();
#define NODE_CREATE(T, V) ({ T _x = V; _node_create(&_x, sizeof(T)); })
#define LIST_APPEND(LST, NN) _list_append(LST, NN);


#ifdef LIST_IMPL
List _list_create(void) { return (List) { .head = NULL }; }
void _list_append(List *lst, Node *nn) {
  if (lst->head == NULL) lst->head = nn;
  else {
    Node *curr = lst->head;
    while (curr->next != NULL) curr = curr->next;
    curr->next = nn;
  }
}
Node *_node_create(void *data, size_t sz) {
  Node *nn = (Node *) malloc(sizeof(Node));
  nn->datasz = sz;
  nn->data = malloc(sz);
  memcpy(nn->data, data, sz);
  return nn;
}
#endif // LIST_IMPL
#endif // LIST_H_
