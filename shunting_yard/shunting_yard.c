#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "shunting_yard.h"

/*   TODO LIST
 *
 *   Brackets are very very buggy, I have to fix that
*/


// function hash definitions
// I simply read the ascii letters as an 8 byte number
#define SIN      7235955
#define COS      7565155
#define TAN      7233908
#define COSEC    426903826275
#define SEC      6514035
#define COT      7630691
#define NEG      126
// end

int StrToSymbols(char stringExpression[], Symbol outputBuffer[], int expressionLength)
{
  int count=1;
  char snippet[8] = {0};
  char* snipPtr = snippet;
  char type=TypeOf(stringExpression[0]);

  *outputBuffer++ = (Symbol) { .type = 'b', .contents.name = '(' };

  char charType;
  int nBrackets[6] = {0};
  for (int i=0; i<expressionLength; ++i) {
    if (snipPtr-snippet > 7) return -1;
    if (!stringExpression[i]) {
      *outputBuffer++ = ParseSnippet(snippet, type);
      *outputBuffer++ = (Symbol) { .type = 'B', .contents.name = ')' };
      if (!(nBrackets[0] == nBrackets[3] && nBrackets[1] == nBrackets[4] && nBrackets[2] == nBrackets[5] )) return -1;
      return (count+2);
    }
    if (stringExpression[i] == ' ' || stringExpression[i] == '\n') continue;

    if ((charType = TypeOf(stringExpression[i])) == type && !(type == 'b' || type == 'B')) {
      if (type == 'F') return -1;
      *snipPtr++ = stringExpression[i];
    }
    else {
      if (charType == 'F' && type == 'b') return -1;
      // changed i-1 to i
      switch (stringExpression[i]) {
        case '(': ++nBrackets[0]; break;
        case '[': ++nBrackets[1]; break;
        case '{': ++nBrackets[2]; break;
        case ')': ++nBrackets[3]; break;
        case ']': ++nBrackets[4]; break;
        case '}': ++nBrackets[5]; break;
      }

      *outputBuffer++ = ParseSnippet(snippet, type);
      type = charType;
      memset(snippet, 0, 8);
      snipPtr = snippet;
      *snipPtr++ = stringExpression[i];
      ++count;
    }
  }

  if (!(nBrackets[0] == nBrackets[3] && nBrackets[1] == nBrackets[4] && nBrackets[2] == nBrackets[5] )) return -1;
  *outputBuffer++ = (Symbol) { .type = 'B', .contents.name = ')' };
  return (count+1);
}

double ShuntingYard(Symbol expression[], int expressionLength, Variable variables[], int variablesLength)
{
  // argument stack
  double numStack[expressionLength];
  double *numStackPtr = numStack;

  // function stack
  Symbol fStack[expressionLength];
  Symbol *fStackPtr = fStack;

  char startingBracket[8] = {0};
  for (int i=0, j=0, invalidVar; i < expressionLength; ++i) {

    switch (expression[i].type) {
      case 'n':
        *numStackPtr++ = expression[i].contents.value;
        break;

      case 'f': case 'F':
        while (
          Precedence(expression[i]) < Precedence(*(fStackPtr-1)) ||
          (Precedence(expression[i]) == Precedence(*(fStackPtr-1)) && IsLeftAssociative(expression[i]))
        ) {
          EvaluateFunction(*--fStackPtr, &numStackPtr);
        }
        *fStackPtr++ = expression[i];
        break;

      case 'v':
        invalidVar = 1;
        for (j=0; j<variablesLength; ++j) {
          if (strncmp(variables[j].name, expression[i].contents.name, 8) == 0) {
            *numStackPtr++ = variables[j].value;
            invalidVar = 0;
            break;
          }
        }
        if (invalidVar) *numStackPtr++ = 0.00; // If variable is not in variables, set it to zero
        break;

      case 'b':
        *fStackPtr++ = expression[i];
        break;

      case 'B':
        GetMatching(expression[i], startingBracket);
        while (strncmp((*(fStackPtr-1)).contents.name, startingBracket, 8) != 0) {
          EvaluateFunction(*--fStackPtr, &numStackPtr);
        }
        --fStackPtr;
        memset(startingBracket, 0, 8);
        break;
    }
  }

  return numStack[0];
}

Symbol ParseSnippet(char snip[8], char type)
{
  Symbol output;
  output.type = type;
  switch (type) {
    case 'n':
      output.contents.value = atof(snip);
      break;
    case 'f': case 'F': case 'v': case 'b': case 'B':
      strncpy(output.contents.name, snip, 8);
      break;
  }
  return output;
}

char TypeOf(char inp)
{
  switch (inp) {
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '.':
      return 'n';
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H':
    case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P':
    case 'Q': case 'R': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y':
    case 'Z':
      return 'v';
    case '(': case '[': case '{':
      return 'b';
    case ')': case ']': case '}':
      return 'B';
    case '+': case '-': case '*': case '/': case '%': case '^':
      return 'F';
    default:
      return 'f';
  }
}

void EvaluateFunction(Symbol function, double **argumentStackPtrPtr)
{
  double arg1, arg2;
  long long *encodedFunc;
  switch (function.type) {
    case 'f':
      encodedFunc = (long long *) function.contents.name;
      arg1 = *--*argumentStackPtrPtr;
      switch (*encodedFunc) {
        case SIN:   *(*argumentStackPtrPtr)++ = sin(arg1); break;
        case COS:   *(*argumentStackPtrPtr)++ = cos(arg1); break;
        case TAN:   *(*argumentStackPtrPtr)++ = (cos(arg1)) ? tan(arg1) : DBL_MAX; break;
        case COSEC: *(*argumentStackPtrPtr)++ = (sin(arg1)) ? 1.00 / sin(arg1) : DBL_MAX; break;
        case SEC:   *(*argumentStackPtrPtr)++ = (cos(arg1)) ? 1.00 / cos(arg1) : DBL_MAX; break;
        case COT:   *(*argumentStackPtrPtr)++ = (sin(arg1)) ? 1.00 / sin(arg1) : DBL_MAX; break;
        case NEG:   *(*argumentStackPtrPtr)++ = -1.0 * arg1; break;
      }
      return;

    case 'F':
      arg2 = *--*argumentStackPtrPtr;
      arg1 = *--*argumentStackPtrPtr;
      switch (function.contents.name[0]) {
        case '+': *(*argumentStackPtrPtr)++ = arg1 + arg2; break;
        case '-': *(*argumentStackPtrPtr)++ = arg1 - arg2; break;
        case '*': *(*argumentStackPtrPtr)++ = arg1 * arg2; break;
        case '^': *(*argumentStackPtrPtr)++ = pow(arg1, arg2); break;
        case '/': *(*argumentStackPtrPtr)++ = (arg2) ? arg1 / arg2 : DBL_MAX; break;
        case '%': *(*argumentStackPtrPtr)++ = (arg2) ? (float)((int)arg1 % (int)arg2) : 0; break;
      }
      return;

    default:
      return;
  }
}

int Precedence(Symbol function)
{
  if (function.type == 'b') return 0;
  if (function.type == 'B') return 127;   // Will never be acessed
  if (function.type == 'f') return 126;
  switch (function.contents.name[0]) {
    case '+': case '-': return 1;
    case '*': case '/': case '%': return 2;
    case '^': return 3;
    default: return 0;
  }
}

int IsLeftAssociative(Symbol function)
{
  if (function.type == 'f') return 0;
  else if (function.contents.name[0] == '^') return 0;
  return 1;
}

void GetMatching(Symbol bracket, char output[8])
{
  switch (bracket.contents.name[0]) {
    case ')':
      output[0] = '(';
      return;
    case '}':
      output[0] = '{';
      return;
    case ']':
      output[0] = '[';
      return;
  }
}
