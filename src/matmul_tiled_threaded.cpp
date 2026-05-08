#include "matmul_tiled_threaded.h"

#include <algorithm>
#include <thread>
#include <vector>
#include <atomic>

void matmul_tiled_threaded(const Matrix& A, const Matrix& B, Matrix& C,
                      int tile_i, int tile_j, int tile_k,
                      int num_threads)
{
  std::atomic<int> next_tile_row {0};

  std::vector<std::thread> threads;
  for(int t = 0; t < num_threads; t++) {
    threads.emplace_back([&]() {
      int tile_row = next_tile_row.fetch_add(tile_i);
      while(tile_row < A.rows()) {
        for(int tile_col = 0; tile_col < B.cols(); tile_col += tile_j) {
          for(int tile_depth = 0; tile_depth < A.cols(); tile_depth += tile_k) {

            //compute the partial dot_products for the tile
            for(int i = tile_row; i < std::min(tile_row + tile_i, A.rows()); i++) {
              for(int j = tile_col; j < std::min(tile_col + tile_j, B.cols()); j++) {
                float dot_prod = C(i,j);
                for(int k = tile_depth; k < std::min(tile_depth + tile_k, A.cols()); k++) {
                  dot_prod += A(i,k) * B(k,j);
                }
                C(i,j) = dot_prod;
              }
            }
          }
        }
        tile_row = next_tile_row.fetch_add(tile_i);
      }
    });
  }

  for(std::thread& t : threads) t.join();
}

