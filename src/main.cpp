#include "matrix.h"
#include "matmul_naive.h"
#include "matmul_tiled_cpu.h"
#include "matmul_tiled_threaded.h"
#include "benchmark.h"

#include <iostream>
#include <thread>

int main()
{
  int tile_i = 32, tile_j = 32, tile_k = 32;
  auto tiled_call = [tile_i, tile_j, tile_k](const Matrix& A, const Matrix& B, Matrix& C) {
    matmul_tiled_cpu(A, B, C, tile_i, tile_j, tile_k);
  };

  int num_threads = std::thread::hardware_concurrency();
  if(num_threads == 0) {
    std::cout << "std::thread::hardware_concurency() returning 0, falling back to 4 threads\n";
    num_threads = 4;
  }
  auto threaded_call = [tile_i, tile_j, tile_k, num_threads](const Matrix& A, const Matrix& B, Matrix& C) {
    matmul_tiled_threaded(A, B, C, tile_i, tile_j, tile_k, num_threads);
  };

  Matrix a = Matrix::random(5,5,42);
  Matrix b = Matrix::random(5,7,100);
  Matrix c = Matrix::zeros(5,7);
  TimingStats stats = time_matmul(matmul_naive, a, b, c, 5);
  print_times("(5x5) X (5x7) (naive)", stats);
  c.zero();
  stats = time_matmul(tiled_call, a, b, c, 5);
  print_times("(5x5) X (5x7) (tiled_cpu)", stats);
  c.zero();
  stats = time_matmul(threaded_call, a, b, c, 5);
  print_times("(5x5) X (5x7) (threaded_cpu)", stats);

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
