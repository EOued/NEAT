#include "testing.h"
#include "macros.h"

int executeTest(test t){
  printf("%s - ", t.test_s);
  t.preoperation(t.object);
  int test =  t.testing(t.object);
  if(t.object) t.freeFunc(t.object);
  return test;
}

void unit_testing(test* t, unsigned int test_n){
  int tests_output = 1;
  int test_output;
  for(unsigned int i = 0; i < test_n; i++){
    test_output = executeTest(t[i]); 
    tests_output &= test_output;
    if(test_output) printf("\x1b[32mTest success\n\x1b[0m");
    else printf("\x1b[31mTest failure\n\x1b[0m");
  }
  FILE* file;
  MEMCHK(file = fopen("unit_test","w+"));
  int out = fprintf(file,"%d", tests_output);
  ERRCHK(out);
  ERRCHK(fclose(file));
  return;
}
