#include "benchmark.h"
#include "matrix.h"

#include <chrono>

double time_matmul(std::function<void(const Matrix&, const Matrix&, Matrix&)> matmul,
                    const Matrix& A, const Matrix& B, Matrix& C, int runs)
{
  matmul(A,B,C);  //run once to warmup the cache

  auto start = std::chrono::steady_clock::now();
  for(int i = 0; i < runs; i++) {
    matmul(A,B,C);
  }
  auto end = std::chrono::steady_clock::now();

  double avg_duration = std::chrono::duration<double>(end - start).count() / runs;

  return avg_duration;
}
