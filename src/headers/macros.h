#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>
#include <stdlib.h>

#define ERR(msg)                                                               \
  do {                                                                         \
    fprintf(stderr, #msg);                                                     \
    exit(1);                                                                   \
  } while (0)

#define ERRCHK(op)                                                             \
  do {                                                                         \
    if ((op) == -1) ERR(op);                                                   \
  } while (0)

#define LYRERRCHK(op)                                                          \
  do {                                                                         \
    if ((op).layer_t == EMPTY) ERR(op);                                        \
  } while (0)

#define MEMCHK(op)                                                             \
  do {                                                                         \
    if ((op) == NULL) ERR(op);                                                 \
  } while (0)

#define REALLOC(size, capacity, size_t, ptr)                                   \
  do {                                                                         \
    if (capacity <= size)                                                      \
    {                                                                          \
      if (!capacity) capacity = 2;                                             \
      else                                                                     \
        capacity *= 2;                                                         \
      MEMCHK(ptr = realloc(ptr, capacity * size_t));                           \
    }                                                                          \
  } while (0)

#define UNIT_TEST(test_arg, test_s_arg, object_arg, preop_args_arg,            \
                  preop_freeing_arg, testing_args_arg, testing_freeing_arg,    \
                  preoperation_arg, testing_arg, free_func_arg,                \
                  testing_args_preop_output_arg)                               \
  do {                                                                         \
    test_arg.test_s                    = test_s_arg;                           \
    test_arg.object                    = object_arg;                           \
    test_arg.preop_args                = preop_args_arg;                       \
    test_arg.preop_freeing             = preop_freeing_arg;                    \
    test_arg.testing_args              = testing_args_arg;                     \
    test_arg.testing_freeing           = testing_freeing_arg;                  \
    test_arg.preoperation              = preoperation_arg;                     \
    test_arg.testing                   = testing_arg;                          \
    test_arg.freeFunc                  = free_func_arg;                        \
    test_arg.testing_args_preop_output = testing_args_preop_output_arg;        \
  } while (0)

#define NULL_LD (layer_descriptor){EMPTY, 0}

#endif
