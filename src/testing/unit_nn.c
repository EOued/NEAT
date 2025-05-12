#include "macros.h"
#include "neural_network.h"
#include "testing.h"

#define TEST_NUMBER 2

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
  unsigned int r2 = network->layers[0].ids_n + rand() % network->layers[1].ids_n;
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

int test_1(void* obj, void* args)
{
  (void)args;
  return ((nn*)obj)->layers[0].ids_n == 5 && ((nn*)obj)->layers[1].ids_n == 5;
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
  test* t;
  MEMCHK(t = malloc(TEST_NUMBER * sizeof(test)));

  // tests
  nn* nn1 = createEmpty(5, 5);
  test t1;
  t1.test_s       = "1.0.1 - Sensor and Output Layer Size";
  t1.preoperation = void_func;
  t1.preop_args   = NULL;
  t1.testing      = test_1;
  t1.testing_args = NULL;
  t1.object       = nn1;
  t1.freeFunc     = freeCallback;
  t1.testing_args_preop_output = 0;
  t[0]            = t1;

  nn* nn2 = createEmpty(2, 3);
  test t2;
  t2.test_s       = "1.1.1 - Connection";
  t2.preoperation = random_connection;
  t2.preop_args   = NULL;
  t2.testing      = test_2;
  t2.testing_args = NULL;
  t2.testing_freeing = free;
  t2.object       = nn2;
  t2.freeFunc     = freeCallback;
  t2.testing_args_preop_output = 1;
  t[1]            = t2;

  // Exec testing
  unit_testing(t, TEST_NUMBER);
  free(t);

  return;
}
