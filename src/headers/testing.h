#ifndef TESTING_H
#define TESTING_H

#include <stdarg.h>
#include <stdio.h>

typedef struct
{
  char* test_s;
  void* object;
  void* preop_args;
  void (*preop_freeing)(void*);
  void* testing_args;
  void (*testing_freeing)(void*);
  void* (*preoperation)(void*, void*);
  int (*testing)(void*, void*);
  void (*freeFunc)(void*);
  int testing_args_preop_output;
} test;

void unit_testing(test* t, unsigned int test_n);
void nn_testing_init(void);

#endif
