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

int test1_0_1(void* obj, void* args)
{
  (void)args;
  return ((nn*)obj)->layers[0].ids_n == 5 && ((nn*)obj)->layers[1].ids_n == 5;
}

int test1_0_2(void* obj, void* args)
{
  (void)args;
  return ((nn*)obj)->layers[0].ids_n == 4 && ((nn*)obj)->layers[1].ids_n == 6;
}

int test1_0_3(void* obj, void* args)
{
  (void)args;
  return ((nn*)obj)->layers[0].ids_n == 50 && ((nn*)obj)->layers[1].ids_n == 5;
}

int test1_0_4(void* obj, void* args)
{
  (void)args;
  return ((nn*)obj)->layers[0].ids_n == 5 && ((nn*)obj)->layers[1].ids_n == 167;
}

int test1_1_1(void* obj, void* args)
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

int test1_1_2(void* obj, void* args)
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

int test1_2(void* obj, void* args)
{
  nn* network     = (nn*)obj;
  unsigned int* r = (unsigned int*)args;
  return network->connections[0].input == r[0] &&
         network->connections[0].output == r[1];
}

void nn_testing_init(void)
{
  unsigned int test_number = 10;

  test* t;
  unsigned int size = 0;
  MEMCHK(t = malloc(test_number * sizeof(test)));

  // tests
  nn* nn1_0_1 = createEmpty(5, 5);
  test t1_0_1;
  UNIT_TEST(t1_0_1, "1.0.1 - Sensor and Output Layer Size", nn1_0_1, NULL,
            void_free, NULL, void_free, void_func, test1_0_1, freeCallback, 0);
  t[size++] = t1_0_1;

  nn* nn1_0_2 = createEmpty(4, 6);
  test t1_0_2;
  UNIT_TEST(t1_0_2, "1.0.2 - Sensor and Output Layer Size", nn1_0_2, NULL,
            void_free, NULL, void_free, void_func, test1_0_2, freeCallback, 0);
  t[size++] = t1_0_2;

  nn* nn1_0_3 = createEmpty(50, 5);
  test t1_0_3;
  UNIT_TEST(t1_0_3, "1.0.3 - Sensor and Output Layer Size", nn1_0_3, NULL,
            void_free, NULL, void_free, void_func, test1_0_3, freeCallback, 0);
  t[size++] = t1_0_3;

  nn* nn1_0_4 = createEmpty(5, 167);
  test t1_0_4;
  UNIT_TEST(t1_0_4, "1.0.4 - Sensor and Output Layer Size", nn1_0_4, NULL,
            void_free, NULL, void_free, void_func, test1_0_4, freeCallback, 0);
  t[size++] = t1_0_4;

  test t1_1_1;
  UNIT_TEST(t1_1_1, "1.1.1 - Invalid Sensor Layer Size", NULL, NULL, void_free,
            NULL, void_free, void_func, test1_1_1, void_free, 0);
  t[size++] = t1_1_1;

  test t1_1_2;
  UNIT_TEST(t1_1_2, "1.1.2 - Invalid Output Layer Size", NULL, NULL, void_free,
            NULL, void_free, void_func, test1_1_2, void_free, 0);
  t[size++] = t1_1_2;

  nn* nn1_2_1 = createEmpty(2, 3);
  test t1_2_1;
  UNIT_TEST(t1_2_1, "1.2.1 - Connection", nn1_2_1, NULL, void_free, NULL, free,
            random_connection, test1_2, freeCallback, 1);
  t[size++] = t1_2_1;

  nn* nn1_2_2 = createEmpty(20, 1);
  test t1_2_2;
  UNIT_TEST(t1_2_2, "1.2.2 - Connection", nn1_2_2, NULL, void_free, NULL, free,
            random_connection, test1_2, freeCallback, 1);
  t[size++] = t1_2_2;

  nn* nn1_2_3 = createEmpty(1, 30);
  test t1_2_3;
  UNIT_TEST(t1_2_3, "1.2.1 - Connection", nn1_2_3, NULL, void_free, NULL, free,
            random_connection, test1_2, freeCallback, 1);
  t[size++] = t1_2_3;

  nn* nn1_2_4 = createEmpty(8, 4);
  test t1_2_4;
  UNIT_TEST(t1_2_4, "1.2.4 - Connection", nn1_2_4, NULL, void_free, NULL, free,
            random_connection, test1_2, freeCallback, 1);
  t[size++] = t1_2_4;

  // Exec testing
  unit_testing(t, test_number);
  free(t);

  return;
}
