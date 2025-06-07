#include <stdlib.h>
#include "toWords.h"

static const char* const numberNames[] = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
                                         "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen",
                                         "eighteen", "nineteen" };
static const char* const denomination[] = { "hundred", "thousand", "million", "billion", "trillion", "quadrillion", "quintillion" };
static const char* const tens[] = { "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety" };

static WordChain* StartChain(const char* word)
{
  WordChain* out = (WordChain *)malloc(sizeof(WordChain));
  if (!out) {
    return NULL;
  }
  out->data = word;
  out->next = NULL;

  return out;
}

static void AddTo(WordChain** chain, const char* word)
{
  if (!chain || !(*chain)) {
    return;
  }

  WordChain* tmp = StartChain(word);
  if (!tmp) {
    return;
  }
  tmp->next = *chain;
  *chain = tmp;
}


WordChain *NumberToWords(unsigned int number)
{
  WordChain* out = StartChain("");
  unsigned char digits[3] = { 0, 0, 0 };
  unsigned int pos = 0;
  while (number) {
    for (int i=0; i<3; ++i) {
      digits[i] = number % 10;
      number /= 10;
    }

    if (pos > 0) {
      AddTo(&out, denomination[pos]);
    }

    if (digits[1] <= 1) {
      if (digits[1] || digits[0])
        AddTo(&out, numberNames[digits[1] * 10 + digits[0]]);
    } else {
      if (digits[0])
        AddTo(&out, numberNames[digits[0]]);
      AddTo(&out, tens[digits[1]-2]);
    }

    if (digits[2]) {
      AddTo(&out, denomination[0]);
      AddTo(&out, numberNames[digits[2]]);
    }
    ++pos;
  }
  return out;
}

void FreeChain(WordChain** chainptr)
{
  if (!(chainptr) || (!*chainptr)) return;
  WordChain* tmp;
  while ((*chainptr)->next) {
    tmp = ((*chainptr)->next);
    free(*chainptr);
    *chainptr = tmp;
  }
  *chainptr = NULL;
}
