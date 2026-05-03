#include "benchmark.h"
#include "matrix.h"

#include <chrono>

double time_matmul(std::function<void(const Matrix&, const Matrix&, Matrix&)> matmul,
                    const Matrix& A, const Matrix& B, Matrix& C, int runs)
{
  double total_duration {0};

  C.zero();       //make sure C is all 0s, so the dot_prods are correct
  matmul(A,B,C);  //run once to warmup the cache

  for(int i = 0; i < runs; i++) {
    C.zero(); //make sure C is all 0's before each run

    auto start = std::chrono::steady_clock::now();
    matmul(A,B,C);
    auto end = std::chrono::steady_clock::now();

    total_duration += std::chrono::duration<double>(end - start).count();
  }

  double avg_duration = total_duration / runs;

  return avg_duration;
}
