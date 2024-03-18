/* Does not compile */
#include <stdlib.h>

void main() {
  int i;
  double A[5] = {
      [0] = 9.0,
      [1] = 2.9,
      [4] = 3.E+25,
      [3] = .0007,
  };

  for (i = 0; i < 5; ++i) {
    printf("element %zu is %g, \tits square is %g\n", i, A[i], A[i] * A[i]);
  }

  return 0;
}
