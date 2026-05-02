#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "matrix.h"

double time_matmul_naive(const Matrix& A, const Matrix& B, Matrix& C, int runs);

#endif
