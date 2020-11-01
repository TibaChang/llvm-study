#include <stdio.h>
int foo(int *A, int n) {
  int b = 0;
#pragma clang loop vectorize_width(4) interleave_count(1)
  for (int i = 0; i < n; ++i) {
    b += A[i];
  }
  return b;
}


int main(void) {
  const int size = 64;
  int A[size];
  for (int i = 0; i < size; i++) {
    A[i] = 1;
  }
  int b = foo(A, size);
  printf("\n\nresult = [%d]\n\n", b);
  return 0;
}
