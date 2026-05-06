#include "benchmark.h"
#include "matrix.h"

#include <chrono>
#include <vector>
#include <cstdio>
#include <string>

using bench_clk   = std::chrono::steady_clock;
using duration_ms = std::chrono::duration<double, std::milli>;

TimingStats time_matmul(std::function<void(const Matrix&, const Matrix&, Matrix&)> matmul,
                    const Matrix& A, const Matrix& B, Matrix& C, int runs)
{
  std::vector<double> times;

  C.zero();       //make sure C is all 0s, so the dot_prods are correct
  matmul(A,B,C);  //run once to warmup the cache

  for(int i = 0; i < runs; i++) {
    C.zero(); //make sure C is all 0's before each run
    auto start = bench_clk::now();
    matmul(A,B,C);
    auto end = bench_clk::now();
    times.push_back(duration_ms(end - start).count());
  }

  double sum = 0.0;
  double min = times[0];
  double max = times[0];

  for(double t : times) {
    sum += t;
    if(t < min) min = t;
    if(t > max) max = t;
  }

  return {min, max, sum / runs};
}

void print_times(const std::string& msg, const TimingStats& stats) {
  std::fprintf(stdout, "%-40s Min: %9.4fms Max: %9.4fms Avg: %9.4fms\n",
               msg.c_str(), stats.min, stats.max, stats.avg);
}
