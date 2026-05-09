#include "matrix.h"
#include "matmul_naive.h"
#include "matmul_tiled_cpu.h"
#include "matmul_tiled_threaded.h"
#include "matmul_naive_gpu.h"
#include "benchmark.h"

#include <iostream>
#include <thread>
#include <cstdio>

auto make_tiled_cpu_fn(int tile_i, int tile_j, int tile_k)
{
  return [tile_i, tile_j, tile_k](const Matrix& left, const Matrix& right, Matrix& result) {
    matmul_tiled_cpu(left, right, result, tile_i, tile_j, tile_k);
  };
}

auto make_tiled_threaded_fn(int tile_i, int tile_j, int tile_k, int num_threads)
{
  return [tile_i, tile_j, tile_k, num_threads](const Matrix& left, const Matrix& right, Matrix& result) {
    matmul_tiled_threaded(left, right, result, tile_i, tile_j, tile_k, num_threads);
  };
}

auto make_naive_gpu_fn()
{
  return [](const Matrix& left, const Matrix& right, Matrix& result) {
    matmul_naive_gpu(left, right, result);
  };
}

void run_benchmark(const Matrix& left, const Matrix& right,
                  int tile_i, int tile_j, int tile_k,
                  int num_threads)
{
  Matrix result = Matrix::zeros(left.rows(), right.cols());

  std::fprintf(stdout,
      "------------------------------------------------------------------------\n"
      "(%dx%d) X (%dx%d)\n",
      left.rows(), left.cols(), right.rows(), right.cols());

  //benchmark naive
  TimingStats stats = time_matmul(matmul_naive, left, right, result, 5);
  print_times("Type: naive", stats);

  //benchmark tiled_cpu
  result.zero();
  stats = time_matmul(make_tiled_cpu_fn(tile_i, tile_j, tile_k), left, right, result, 5);
  print_times("Type: tiled_cpu", stats);

  //benchmark matmul_tiled_threaded
  result.zero();
  stats = time_matmul(make_tiled_threaded_fn(tile_i, tile_j, tile_k, num_threads), left, right, result, 5);
  print_times("Type: tiled_threaded", stats);

  //benchmark matmul_naive_gpu
  result.zero();
  stats = time_matmul(make_naive_gpu_fn(), left, right, result, 5);
  print_times("Type: naive_gpu", stats);
}

int main()
{
  int num_threads = std::thread::hardware_concurrency();
  if(num_threads == 0) {
    std::cout << "std::thread::hardware_concurency() returning 0, falling back to 4 threads\n";
    num_threads = 4;
  }
  else {
    std::cout << "Using " << std::to_string(num_threads) << " threads\n";
  }

  run_benchmark(Matrix::random(5,5,42), Matrix::random(5,7,100),
                /*tile_size =*/ 32, 32, 32,
                num_threads);

  run_benchmark(Matrix::random(100,500,47), Matrix::random(500, 100, 91),
                /*tile_size =*/ 32, 32, 32,
                num_threads);

  run_benchmark(Matrix::random(1000,1025,56), Matrix::random(1025, 900, 77),
                /*tile_size =*/ 32, 32, 32,
                num_threads);

  run_benchmark(Matrix::random(2048,1587,66), Matrix::random(1587, 1024, 111),
                /*tile_size =*/ 32, 32, 32,
                num_threads);

  return 0;
}
