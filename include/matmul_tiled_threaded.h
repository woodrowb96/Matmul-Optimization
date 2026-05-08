#ifndef MATMUL_TILED_THREADED_H
#define MATMUL_TILED_THREADED_H

#include "matrix.h"

//NOTE: C must be zero-initialized before its passed into this function
//        - The dot_product uses C(i,j) += to accumulate the multiplications
//          so any non-zero values in C at the start will corrput the result
void matmul_tiled_threaded(const Matrix& A, const Matrix& B, Matrix& C,
                    int tile_i, int tile_j, int tile_k,
                    int num_threads);
#endif
