#include "testing.h"
#include "macros.h"

int executeTest(test t){
  t.preoperation(t.object);
  return t.testing(t.object);
}

void unit_testing(test* t, unsigned int test_n){
  int test_output = 1;
  for(unsigned int i = 0; i < test_n; i++){
    test_output &= executeTest(t[i]); 
  }
  FILE* file;
  MEMCHK(file = fopen("unit_test","w+"));
  int out = fprintf(file,"%d", test_output);
  ERRCHK(out);
  ERRCHK(fclose(file));
  return;
}
