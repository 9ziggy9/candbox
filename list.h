#ifndef LIST_H_
#define LIST_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

typedef struct Node {
  void *data;
  size_t datasz;
  struct Node *next;
} Node;
typedef struct List { Node *head; } List;

#define LIST_CREATE() _list_create();

// This is a GNU extension which clang also provides! Beware.
#define LIST_NODE_CREATE(T, V)                       \
  ({ T _x = V; _list_node_create(&_x, sizeof(T)); }) \

#define LIST_APPEND(LST, NN) _list_append(LST, NN);

List _list_create(void);
Node *_list_node_create(void *, size_t);
void _list_append(List *, Node *);

#ifdef LIST_IMPL
List _list_create(void) { return (List) { .head = NULL }; }

Node *_list_node_create(void *data, size_t sz) {
  Node *nn = (Node *) malloc(sz); // TODO: error handling
  nn->datasz = sz;
  nn->data = malloc(sz);
  memcpy(nn->data, data, sz);
  return nn;
}

void _list_append(List *lst, Node *nn) {
  Node *curr = lst->head;
  if (curr == NULL) lst->head = nn;
  else {
    while(curr->next != NULL) curr = curr->next;
    curr->next = nn;
  }
}

void _list_print(const char *fmt, List *lst) {
  Node *curr = lst->head;
  while (curr != NULL) {
    char formatted[256];
    sprintf(formatted, fmt, *(int * ) curr->data);
    printf("[ %s ]", formatted);
    printf("%s", curr->next == NULL ? " -> NULL\n" : " -> ");
    curr = curr->next;
  }
}

#endif // LIST_IMPL
#endif // LIST_H_
