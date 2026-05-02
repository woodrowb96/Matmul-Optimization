#include "matrix.h"
#include "matmul_naive.h"
#include "benchmark.h"

#include <iostream>

int main()
{
  Matrix a = Matrix::random(5,5,42);
  Matrix b = Matrix::random(5,7,100);
  Matrix c (5,7);
  double time = time_matmul_naive(a, b, c, 10);
  std::cout << " (5x5) X (5x7) avg time: " << time << "\n";

  Matrix d = Matrix::random(100, 500, 47);
  Matrix e = Matrix::random(500, 100, 91);
  Matrix f (100, 100);
  time = time_matmul_naive(d, e, f, 10);
  std::cout << " (100x500) X (500x100) avg time: " << time << "\n";

  Matrix g = Matrix::random(1000, 1025, 47);
  Matrix h = Matrix::random(1025, 900, 91);
  Matrix i (1000, 900);
  time = time_matmul_naive(g, h, i, 10);
  std::cout << " (1000x1025) X (1025x900) avg time: " << time << "\n";

  Matrix j = Matrix::random(2048, 1587, 49);
  Matrix k = Matrix::random(1587, 1024, 10000);
  Matrix l (2048, 1024);
  time = time_matmul_naive(j, k, l, 10);
  std::cout << " (2048, 1587) X (1587x1024) avg time: " << time << "\n";

  return 0;
}
