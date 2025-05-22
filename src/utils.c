#include "utils.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int next_power_of_two(unsigned int x)
{
  if (x == 0) return 1;

  x--;
  for (unsigned int i = 1; i < sizeof(x) * CHAR_BIT; i <<= 1) x |= x >> i;
  return x + 1;
}

int random_between_range(int a, int b) { return a + rand() % (b - a + 1); }
