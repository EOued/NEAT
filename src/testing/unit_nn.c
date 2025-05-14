#include "macros.h"
#include "neural_network.h"
#include "testing.h"

#include <fcntl.h>
#include <setjmp.h>
#include <stdio.h>
#include <unistd.h>

// stderr redirection

int saved_stderr;

void disable_stderr(void)
{
  saved_stderr = dup(fileno(stderr)); // Save original stderr
  freopen("/dev/null", "w", stderr);
}

void restore_stderr(void)
{
  fflush(stderr);
  dup2(saved_stderr, fileno(stderr)); // Restore original stderr
  close(saved_stderr);
}

// exit redefinition

jmp_buf exit_jmp_buf;
int exit_code;

void exit(int code)
{
  exit_code = code;
  longjmp(exit_jmp_buf, 1);
}

// preops

void* void_func(void* obj, void* args)
{
  (void)obj;
  (void)args;
  return NULL;
}

void* random_connection(void* obj, void* args)
{
  (void)args;
  nn* network     = (nn*)obj;
  unsigned int r1 = rand() % network->layers[0].ids_n;
  unsigned int r2 =
      network->layers[0].ids_n + rand() % network->layers[1].ids_n;
  unsigned int* r;
  MEMCHK(r = malloc(2 * sizeof(int)));
  r[0] = r1;
  r[1] = r2;
  addConnection(network, (connection){r1, r2, 0});
  return r;
}

// free

void freeCallback(void* nn)
{
  freeNN(nn);
  return;
}

void void_free(void* obj)
{
  (void)obj;
  return;
}

// tests

int test1_0(void* obj, void* args)
{
  (void)args;
  return ((nn*)obj)->layers[0].ids_n == 5 && ((nn*)obj)->layers[1].ids_n == 5;
}

int test1_1(void* obj, void* args)
{
  (void)obj;
  (void)args;
  disable_stderr();
  if (setjmp(exit_jmp_buf) == 0)
  {
    createEmpty(0, 1);
    restore_stderr();
    return 0;
  }
  restore_stderr();
  return 1;
}

int test1_2(void* obj, void* args)
{
  (void)obj;
  (void)args;
  disable_stderr();
  if (setjmp(exit_jmp_buf) == 0)
  {
    createEmpty(1, 0);
    restore_stderr();
    return 0;
  }
  restore_stderr();
  return 1;
}

int test_2(void* obj, void* args)
{
  nn* network     = (nn*)obj;
  unsigned int* r = (unsigned int*)args;
  return network->connections[0].input == r[0] &&
         network->connections[0].output == r[1];
}

void nn_testing_init(void)
{
  unsigned int test_number = 4;

  test* t;
  unsigned int size = 0;
  MEMCHK(t = malloc(test_number * sizeof(test)));

  // tests
  nn* nn1_0 = createEmpty(5, 5);
  test t1_0;

  UNIT_TEST(t1_0, "1.0.1 - Sensor and Output Layer Size", nn1_0, NULL,
            void_free, NULL, void_free, void_func, test1_0, freeCallback, 0);
  t[size++] = t1_0;

  test t1_1;
  UNIT_TEST(t1_1, "1.0.2 - Invalid Sensor Layer Size", NULL, NULL, void_free,
            NULL, void_free, void_func, test1_1, void_free, 0);
  t[size++] = t1_1;

  test t1_2;
  UNIT_TEST(t1_2, "1.0.3 - Invalid Output Layer Size", NULL, NULL, void_free,
            NULL, void_free, void_func, test1_2, void_free, 0);
  t[size++] = t1_2;

  nn* nn2 = createEmpty(2, 3);

  test t2;
  UNIT_TEST(t2, "1.1.1 - Connection", nn2, NULL, void_free, NULL, free,
            random_connection, test_2, freeCallback, 1);
  t[size++] = t2;

  // Exec testing
  unit_testing(t, test_number);
  free(t);

  return;
}
