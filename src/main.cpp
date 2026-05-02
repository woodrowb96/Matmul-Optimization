#include "matrix.h"
#include "matmul_naive.h"
#include "matmul_tiled_cpu.h"
#include "benchmark.h"

#include <iostream>

int main()
{
  // Matrix a = Matrix::random(5,5,42);
  // Matrix b = Matrix::random(5,7,100);
  // Matrix c (5,7);
  // double time = time_matmul_naive(a, b, c, 10);
  // std::cout << " (5x5) X (5x7) avg time: " << time << "\n";
  //
  // Matrix d = Matrix::random(100, 500, 47);
  // Matrix e = Matrix::random(500, 100, 91);
  // Matrix f (100, 100);
  // time = time_matmul_naive(d, e, f, 10);
  // std::cout << " (100x500) X (500x100) avg time: " << time << "\n";
  //
  // Matrix g = Matrix::random(1000, 1025, 47);
  // Matrix h = Matrix::random(1025, 900, 91);
  // Matrix i (1000, 900);
  // time = time_matmul_naive(g, h, i, 10);
  // std::cout << " (1000x1025) X (1025x900) avg time: " << time << "\n";
  //
  // Matrix j = Matrix::random(2048, 1587, 49);
  // Matrix k = Matrix::random(1587, 1024, 10000);
  // Matrix l (2048, 1024);
  // time = time_matmul_naive(j, k, l, 10);
  // std::cout << " (2048, 1587) X (1587x1024) avg time: " << time << "\n";

  Matrix m (2,2,{1.0f,1.0f,1.0f,1.0f});
  Matrix n (2,2,{1.0f,1.0f,1.0f,1.0f});
  Matrix o (2,2);
  matmul_tiled_cpu(m, n, o, 16, 16, 16);
  std::cout << "o:\n";
  o.print();

  Matrix p (2,4,{5.0f,10.0f,18.0f,1.0f, 5.0f, 3.0f, 3.0f, 6.0f });
  Matrix q (4,3,{3.0f,-3.0f,9.0f,6.0f, 4.0f, 78.0f, 83.0f, 22.0f, 96.0f, 1.0f, 6.0f, 5.0f});
  Matrix r (2,3);
  matmul_tiled_cpu(p, q, r, 16, 16, 16);
  std::cout << "r:\n";
  r.print();


  return 0;
}
