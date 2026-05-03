#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "matrix.h"

#include <functional>

double time_matmul(std::function<void(const Matrix&, const Matrix&, Matrix&)> matmul,
                    const Matrix& A, const Matrix& B, Matrix& C, int runs);

#endif
