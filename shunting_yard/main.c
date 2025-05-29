#include <stdio.h>
#include <stddef.h>
#include "shunting_yard.h"

#define MAX_SYMBOLS 1024


int main()
{
  Symbol input_expression[MAX_SYMBOLS];
  char inp[100];
  scanf("%s", inp);
  int input_size = StrToSymbols(inp, input_expression, 100);
  if (input_size == -1) {
    printf("Input expression is invalid!\n");
    return 1;
  }

  Variable vars[1] = { { "X", 7 } };
  printf("Result: %f\n", ShuntingYard(input_expression, input_size, vars, 1));
  return 0;
}
