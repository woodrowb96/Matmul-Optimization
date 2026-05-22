#ifndef MATMUL_TILED_REG_GPU
#define MATMUL_TILED_REG_GPU

#include "matrix.h"

//Later ill add a template to get tunable tile sizes
void matmul_tiled_reg_gpu(const Matrix& l, const Matrix& r, Matrix& res);

#endif

