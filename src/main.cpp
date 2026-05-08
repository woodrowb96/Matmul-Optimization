#include "matrix.h"
#include "matmul_naive.h"
#include "matmul_tiled_cpu.h"
#include "matmul_tiled_threaded.h"
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
}

int main()
{
  int num_threads = std::thread::hardware_concurrency();
  if(num_threads == 0) {
    std::cout << "std::thread::hardware_concurency() returning 0, falling back to 4 threads\n";
    num_threads = 4;
  }
  run_benchmark(Matrix::random(5,5,42), Matrix::random(5,7,100),
                /*tile_size =*/ 32, 32, 32,
                num_threads);

  // Matrix d = Matrix::random(100, 500, 47);
  // Matrix e = Matrix::random(500, 100, 91);
  // Matrix f (100, 100);
  // stats = time_matmul(matmul_naive, d, e, f, 5);
  // print_times("(100x500) X (500x100) (naive)", stats);
  // f.zero();
  // stats = time_matmul(tiled_call, d, e, f, 5);
  // print_times("(100x500) X (500x100) (tiled_cpu)", stats);
  //
  // Matrix g = Matrix::random(1000, 1025, 47);
  // Matrix h = Matrix::random(1025, 900, 91);
  // Matrix i (1000, 900);
  // stats = time_matmul(matmul_naive, g, h, i, 5);
  // print_times("(1000x1025) X (1025x900) (naive)", stats);
  // i.zero();
  // stats = time_matmul(tiled_call, g, h, i, 5);
  // print_times("(1000x1025) X (1025x900) (tiled_cpu)", stats);
  //
  // Matrix j = Matrix::random(2048, 1587, 49);
  // Matrix k = Matrix::random(1587, 1024, 10000);
  // Matrix l (2048, 1024);
  // stats = time_matmul(matmul_naive, j, k, l, 5);
  // print_times("(2048x1587) X (1587x1024) (naive)", stats);
  // l.zero();
  // stats = time_matmul(tiled_call, j, k, l, 5);
  // print_times("(2048x1587) X (1587x1024) (tiled_cpu)", stats);

  return 0;
}
