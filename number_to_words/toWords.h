#pragma once

typedef struct WordChain WordChain;
struct WordChain {
  const char* data;
  WordChain* next;
};


WordChain *NumberToWords(unsigned int number);
void FreeChain(WordChain** chainptr);
