#include <stdio.h>

const int size = 64;

int foo(int *A) {
  int b = 0;
#pragma clang loop vectorize_width(4) interleave_count(1)
  for (int i = 0; i < size; i++) {
    b += A[i];
  }
  return b;
}


int main(void) {
  int A[size];
  for (int i = 0; i < size; i++) {
    A[i] = 1;
  }
  int b = foo(A);
  printf("\n\nresult = [%d]\n\n", b);
  return 0;
}
