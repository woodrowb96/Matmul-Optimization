#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "matrix.h"

#include <functional>
#include <string>

struct TimingStats
{
  double min {-1.0};
  double max {-1.0};
  double avg {-1.0};
};

TimingStats time_matmul(std::function<void(const Matrix&, const Matrix&, Matrix&)> matmul,
                    const Matrix& A, const Matrix& B, Matrix& C, int runs);

void print_times(const std::string& msg, const TimingStats& stats);

#endif
