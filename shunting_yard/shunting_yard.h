#pragma once

// This is 16 bytes because of padding smh
// Should probably do something to take care of symbols
// and variables under the same struct later
typedef struct Symbol {
  char type;
  union data {
    double value;
    char name[8];
  } contents;
} Symbol;

typedef struct Variable {
  char name[8];
  double value;
} Variable;

int StrToSymbols(char stringExpression[], Symbol outputBuffer[], int expressionLength);
Symbol ParseSnippet(char snip[8], char type);
char TypeOf(char inp);

double ShuntingYard(Symbol expression[], int expressionLength, Variable variables[], int variableLength);
void EvaluateFunction(Symbol function, double **argumentStackPtr);
int Precedence(Symbol function);
int IsLeftAssociative(Symbol function);
void GetMatching(Symbol bracket, char output[8]);
