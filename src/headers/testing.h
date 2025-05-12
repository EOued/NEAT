#ifndef TESTING_H
#define TESTING_H

#include <stdarg.h>
#include <stdio.h>

typedef struct
{
  void* object;
  void* (*preoperation)(void*);
  int (*testing)(void*);
} test;

void unit_testing(test* t, unsigned int test_n);

#endif
