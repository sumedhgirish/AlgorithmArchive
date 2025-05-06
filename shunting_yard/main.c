#include <stdio.h>
#include <stddef.h>
#include "shunting_yard.h"

#define MAX_SYMBOLS 1024


int main(int argc, char* argv[])
{
  if (argc != 2) {
    printf("Usage: %s <expression without spaces>\n", argv[0]);
    return 1;
  }

  Symbol input_expression[MAX_SYMBOLS];
  int input_size = StrToSymbols(argv[1], input_expression, sizeof(argv[1]));

  printf("Result: %f\n", ShuntingYard(input_expression, input_size, NULL, 0));
  return 0;
}
