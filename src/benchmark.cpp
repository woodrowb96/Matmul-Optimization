#include "benchmark.h"
#include "matrix.h"
#include "matmul_naive.h"

#include <chrono>

double time_matmul_naive(const Matrix& A, const Matrix& B, Matrix& C, int runs)
{
  matmul_naive(A,B,C);  //run once to warmup the cache

  auto start = std::chrono::steady_clock::now();
  for(int i = 0; i < runs; i++) {
    matmul_naive(A,B,C);
  }
  auto end = std::chrono::steady_clock::now();

  double avg_duration = std::chrono::duration<double>(end - start).count() / runs;

  return avg_duration;
}
