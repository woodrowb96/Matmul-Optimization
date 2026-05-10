#include "matmul_tiled_gpu.h"
#include "matrix.h"

#include <cassert>

//Hardcode the tile size for now, Ill add templates later to get tunable tile sizes
constexpr int TILE_I = 16; //num rows in tile
constexpr int TILE_J = 16; //num cols in tile
constexpr int TILE_K = 16; //num cols in tile

// l is (m x k), r is (k x n) and res is (m x n)
__global__ void matmul_tiled_gpu_kernal(const float* l, const float* r, float* res,
                                        int m, int k, int n)
{
  __shared__ float l_tile[TILE_I][TILE_K];
  __shared__ float r_tile[TILE_K][TILE_J];

  int row = blockIdx.y * TILE_I + threadIdx.y;
  int col = blockIdx.x * TILE_J + threadIdx.x;

  float sum = 0.0f;

  for (int kk = 0; kk < k; kk += TILE_K) {

    if (row < m && (kk + threadIdx.x) < k)
      l_tile[threadIdx.y][threadIdx.x] = l[row * k + kk + threadIdx.x];
    else
      l_tile[threadIdx.y][threadIdx.x] = 0.0f;

    if ((kk + threadIdx.y) < k && col < n)
      r_tile[threadIdx.y][threadIdx.x] = r[(kk + threadIdx.y) * n + col];
    else
      r_tile[threadIdx.y][threadIdx.x] = 0.0f;

    __syncthreads();

    for (int t = 0; t < TILE_K; t++) {
      sum += l_tile[threadIdx.y][t] * r_tile[t][threadIdx.x];
    }

    __syncthreads();
  }

  if (row < m && col < n) {
    res[row * n + col] = sum;
  }
}

void matmul_tiled_gpu(const Matrix& l, const Matrix& r, Matrix& res)
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

  //TO DO
dim3 block_dim(TILE_J, TILE_I);
dim3 grid_dim((n + TILE_J - 1) / TILE_J,
              (m + TILE_I - 1) / TILE_I);
  matmul_tiled_gpu_kernal<<<grid_dim, block_dim>>>(d_l, d_r, d_res, m, k, n);
  cudaDeviceSynchronize();

  cudaMemcpy(res.data(), d_res, res_bytes, cudaMemcpyDeviceToHost);

  cudaFree(d_l);
  cudaFree(d_r);
  cudaFree(d_res);
}
