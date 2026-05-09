#include "matmul_naive_gpu.h"
#include "matrix.h"

#include <cassert>

// l is (m x k), r is (k x n) and res is (m x n)
__global__ void matmul_naive_gpu_kernal(const float* l, const float* r, float* res,
                                        int m, int k, int n)
{
  int i = blockIdx.y * blockDim.y + threadIdx.y;
  int j = blockIdx.x * blockDim.x + threadIdx.x;

  if(i < m && j < n) {
    float sum = 0.0f;
    for(int kk = 0; kk < k; kk++) {
      sum += l[(i * k) + kk] * r[(kk * n) + j];
    }
    res[(i * n) + j] = sum;
  }
}

void matmul_naive_gpu(const Matrix& l, const Matrix& r, Matrix& res)
{
  int m = l.rows();
  int k = l.cols();
  int n = r.cols();
  assert(r.rows() == k); //make sure inner dimensions match

  size_t l_bytes   = m * k * sizeof(float);
  size_t r_bytes   = k * n * sizeof(float);
  size_t res_bytes = m * n * sizeof(float);

  float* d_l   {nullptr};  cudaMalloc(&d_l,   l_bytes);
  float* d_r   {nullptr};  cudaMalloc(&d_r,   r_bytes);
  float* d_res {nullptr};  cudaMalloc(&d_res, res_bytes);

  cudaMemcpy(d_l, l.data(), l_bytes, cudaMemcpyHostToDevice);
  cudaMemcpy(d_r, r.data(), r_bytes, cudaMemcpyHostToDevice);

  dim3 block_dim(16, 16);
  dim3 grid_dim((n + 15) / 16, (m + 15) / 16);
  matmul_naive_gpu_kernal<<<grid_dim, block_dim>>>(d_l, d_r, d_res, m, k, n);
  cudaDeviceSynchronize();

  cudaMemcpy(res.data(), d_res, res_bytes, cudaMemcpyDeviceToHost);

  cudaFree(d_l);
  cudaFree(d_r);
  cudaFree(d_res);
}
