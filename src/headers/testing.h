#ifndef TESTING_H
#define TESTING_H

#include <stdarg.h>
#include <stdio.h>

typedef struct
{
  char* test_s;
  void* object;
  void* (*preoperation)(void*);
  int (*testing)(void*);
  void (*freeFunc)(void*);
} test;

void unit_testing(test* t, unsigned int test_n);
void nn_testing_init(void);

#endif
