#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>
#include <stdlib.h>

#define ERR(msg)                                                               \
  do {                                                                         \
    perror(#msg);                                                              \
    exit(1);                                                                   \
  } while (0)

#define ERRCHK(op)                                                             \
  do {                                                                         \
    if ((op) == -1) ERR(op);                                                   \
  } while (0)

#define MEMCHK(op)                                                             \
  do {                                                                         \
    if ((op) == NULL) ERR(op);                                                 \
  } while (0)

#define REALLOC(size, capacity, size_t, ptr)                                   \
  do {                                                                         \
    if (capacity < size)                                                       \
    {                                                                          \
      capacity *= 2;                                                           \
      MEMCHK(ptr = realloc(ptr, capacity * size_t));                           \
    }                                                                          \
  } while (0)

#endif
