#include "matmul_tiled_threaded.h"

#include <algorithm>
#include <thread>
#include <vector>
#include <atomic>

void matmul_tiled_threaded(const Matrix& A, const Matrix& B, Matrix& C,
                      int tile_i, int tile_j, int tile_k,
                      int num_threads)
{
  std::atomic<int> next_tile_i {0};

  std::vector<std::thread> threads;
  for(int i = 0; i < num_threads; i++) {
    threads.emplace_back([&]() {
      while(true) {
        int current_tile_i = next_tile_i.fetch_add(tile_i);
        if(current_tile_i >= A.rows()) break;
        compute_tile_row(A, B, C, tile_i, tile_j, tile_k, current_tile_i);
      }
    });
  }

  for(std::thread& t : threads) {
    t.join();
  }
}

void compute_tile_row(const Matrix& A, const Matrix& B, Matrix& C,
                      int tile_i, int tile_j, int tile_k,   //each tile is iXjxK
                      int current_tile_i)
{
  for(int jj = 0; jj < B.cols(); jj += tile_j) {
    for(int kk = 0; kk < A.cols(); kk += tile_k) {

      for(int i = current_tile_i; i < std::min(current_tile_i + tile_i, A.rows()); i++) {
        for(int j = jj; j < std::min(jj + tile_j, B.cols()); j++) {
          float dot_prod = C(i,j);
          for(int k = kk; k < std::min(kk + tile_k, A.cols()); k++) {
            dot_prod += A(i,k) * B(k,j);
          }
          C(i,j) = dot_prod;
        }
      }

    }
  }
}
