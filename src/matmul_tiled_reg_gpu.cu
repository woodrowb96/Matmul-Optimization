#include "matmul_tiled_reg_gpu.h"
#include "matrix.h"

#include <cassert>

// Each block computes a BLOCK_M x BLOCK_N output tile.
// Each thread computes a REG_M x REG_N register block of that tile.
// So threads/block = (BLOCK_M / REG_M) x (BLOCK_N / REG_N) = 16x16 = 256.
constexpr int BLOCK_M = 64;
constexpr int BLOCK_N = 64;
constexpr int TILE_K  = 16;
constexpr int REG_M   = 4;
constexpr int REG_N   = 4;

constexpr int THREADS_Y         = BLOCK_M / REG_M;
constexpr int THREADS_X         = BLOCK_N / REG_N;
constexpr int THREADS_PER_BLOCK = THREADS_Y * THREADS_X;

// l is (m x k), r is (k x n) and res is (m x n)
__global__ void matmul_tiled_reg_gpu_kernal(const float* l, const float* r, float* res,
                                            int m, int k, int n)
{
  __shared__ float l_tile[BLOCK_M][TILE_K];
  __shared__ float r_tile[TILE_K][BLOCK_N];

  int tid = threadIdx.y * THREADS_X + threadIdx.x;

  int block_row = blockIdx.y * BLOCK_M;
  int block_col = blockIdx.x * BLOCK_N;

  // This thread's top-left output cell (in global coords)
  int thread_row = block_row + threadIdx.y * REG_M;
  int thread_col = block_col + threadIdx.x * REG_N;

  // Per-thread accumulator block, lives in registers
  float acc[REG_M][REG_N];
  for (int i = 0; i < REG_M; i++)
    for (int j = 0; j < REG_N; j++)
      acc[i][j] = 0.0f;

  for (int kk = 0; kk < k; kk += TILE_K) {

    // Cooperative load of l_tile (BLOCK_M x TILE_K = 1024 elems via 256 threads = 4 each).
    // Strided so consecutive threads hit consecutive cells.
    for (int e = tid; e < BLOCK_M * TILE_K; e += THREADS_PER_BLOCK) {
      int local_row  = e / TILE_K;
      int local_col  = e % TILE_K;
      int global_row = block_row + local_row;
      int global_col = kk + local_col;
      l_tile[local_row][local_col] =
          (global_row < m && global_col < k) ? l[global_row * k + global_col] : 0.0f;
    }

    // Cooperative load of r_tile (TILE_K x BLOCK_N = 1024 elems via 256 threads = 4 each)
    for (int e = tid; e < TILE_K * BLOCK_N; e += THREADS_PER_BLOCK) {
      int local_row  = e / BLOCK_N;
      int local_col  = e % BLOCK_N;
      int global_row = kk + local_row;
      int global_col = block_col + local_col;
      r_tile[local_row][local_col] =
          (global_row < k && global_col < n) ? r[global_row * n + global_col] : 0.0f;
    }

    __syncthreads();

    // Inner-K loop. Each iteration loads two short fragments into registers, then
    // does REG_M*REG_N MACs. Each shared-memory load is reused REG_M (or REG_N) times.
    for (int t = 0; t < TILE_K; t++) {
      float a_frag[REG_M];
      float b_frag[REG_N];

      for (int i = 0; i < REG_M; i++)
        a_frag[i] = l_tile[threadIdx.y * REG_M + i][t];

      for (int j = 0; j < REG_N; j++)
        b_frag[j] = r_tile[t][threadIdx.x * REG_N + j];

      for (int i = 0; i < REG_M; i++)
        for (int j = 0; j < REG_N; j++)
          acc[i][j] += a_frag[i] * b_frag[j];
    }

    __syncthreads();
  }

  // Write each thread's REG_M x REG_N register block out to global memory
  for (int i = 0; i < REG_M; i++) {
    for (int j = 0; j < REG_N; j++) {
      int row = thread_row + i;
      int col = thread_col + j;
      if (row < m && col < n)
        res[row * n + col] = acc[i][j];
    }
  }
}

void matmul_tiled_reg_gpu(const Matrix& l, const Matrix& r, Matrix& res)
{
  int m = l.rows();
  int k = l.cols();
  int n = r.cols();
  assert(r.rows() == k);

  size_t l_bytes   = m * k * sizeof(float);
  size_t r_bytes   = k * n * sizeof(float);
  size_t res_bytes = m * n * sizeof(float);

  float* d_l   {nullptr};  cudaMalloc(&d_l,   l_bytes);
  float* d_r   {nullptr};  cudaMalloc(&d_r,   r_bytes);
  float* d_res {nullptr};  cudaMalloc(&d_res, res_bytes);

  cudaMemcpy(d_l, l.data(), l_bytes, cudaMemcpyHostToDevice);
  cudaMemcpy(d_r, r.data(), r_bytes, cudaMemcpyHostToDevice);

  dim3 block_dim(THREADS_X, THREADS_Y);
  dim3 grid_dim((n + BLOCK_N - 1) / BLOCK_N,
                (m + BLOCK_M - 1) / BLOCK_M);
  matmul_tiled_reg_gpu_kernal<<<grid_dim, block_dim>>>(d_l, d_r, d_res, m, k, n);
  cudaDeviceSynchronize();

  cudaMemcpy(res.data(), d_res, res_bytes, cudaMemcpyDeviceToHost);

  cudaFree(d_l);
  cudaFree(d_r);
  cudaFree(d_res);
}
