#include <stdio.h>
#include "kmp_searching.h"

int main()
{
  printf("Found the key at: %d\n", findStr("GOLIATH", "DAVID & GOLIATH", 7, 15));
  return 0;
}
