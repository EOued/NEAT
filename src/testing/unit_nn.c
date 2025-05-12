#include "macros.h"
#include "neural_network.h"
#include "testing.h"

#define TEST_NUMBER 1

void* void_func(void* obj)
{
  (void)obj;
  return NULL;
}

void freeCallback(void* nn)
{
  freeNN(nn);
  return;
}

int test_1(void* obj)
{
  return ((nn*)obj)->layers[0].ids_n == 5 && ((nn*)obj)->layers[1].ids_n == 5;
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
  t1.testing      = test_1;
  t1.object       = nn1;
  t1.freeFunc     = freeCallback;
  t[0]            = t1;
  // Exec testing
  unit_testing(t, TEST_NUMBER);
  free(t);

  return;
}
