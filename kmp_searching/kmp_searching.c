// https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm
#include "kmp_searching.h"


int findStr(char key[], char content[], int key_length, int content_length)
{
  int m=0, i=0;
  while (m<content_length) {
    for (i=0; i<key_length; ++i) {
      if (content[m+i] != key[i]) {
        m += (i > 0) ? i - CoherenceLength(key, i) : 1;
        break;
      }
    }
    if (i == key_length) return m;
  }
  return -1;
}

int CoherenceLength(char *key, int k)
{
  int j;
  for (int i=0; i<k; ++i) {
    for (j=0; j<(k-i); ++j) {
      if (key[i+j] != key[j]) break;
    }
    if (j == k-i) return k-i;
  }
  return 0;
}
