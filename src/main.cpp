#include "matrix.h"
#include "matmul_naive.h"

#include <iostream>

int main()
{
  Matrix a (2, 2, {1.0f, 2.0f, 1.0f, 2.0f});
  Matrix b (2, 4, {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f});
  Matrix c (2, 4);

  matmul_naive(a, b, c); //expect ( 3 3 3 3, 3 3 3 3)
  c.print();
  std::cout << "\n";

  Matrix d (3, 2, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f});
  Matrix e (2, 4, {2.0f, 2.0f, 3.0f, 3.0f, 4.0f, 4.0f, 5.0f, 5.0f});
  Matrix f (3, 4);

  matmul_naive(d, e, f); //expect ( 10 10 13 13, 22 22 29 29, 34 34 45 45)
  f.print();
  return 0;
}
