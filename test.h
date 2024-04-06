#ifndef TEST_H_
#define TEST_H_

#define TEST_ARRAY_INIT(V) do {                             \
  Array xs = ARRAY_INIT(V, ((int []){1, 2, 3, 4, 5}))       \
  array_push(&xs, 1);                                       \
  array_push(&xs, 2);                                       \
  array_push(&xs, 3);                                       \
  array_push(&xs, 4);                                       \
  array_print(&xs);                                         \
} while(0)                                                  \

#define TEST_ARRAY_NEW(V) do {                              \
  Array xs = ARRAY_NEW(V, 3)                                \
  array_push(&xs, 1);                                       \
  array_push(&xs, 2);                                       \
  array_push(&xs, 3);                                       \
  array_push(&xs, 4);                                       \
  array_print(&xs);                                         \
} while(0)                                                  \

#endif // TEST_H_
