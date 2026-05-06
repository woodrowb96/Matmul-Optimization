#include "matmul_tiled_cpu.h"

#include<algorithm>

void matmul_tiled_cpu(const Matrix& A, const Matrix& B, Matrix& C,
                      int tile_i, int tile_j, int tile_k)
{
  for(int ii = 0; ii < A.rows(); ii += tile_i) {
    for(int jj = 0; jj < B.cols(); jj += tile_j) {
      for(int kk = 0; kk < A.cols(); kk += tile_k) {

        /************************************************************/
        for(int i = ii; i < std::min(ii + tile_i, A.rows()); i++) {
          for(int j = jj; j < std::min(jj + tile_j, B.cols()); j++) {
            float dot_prod = C(i,j);
            for(int k = kk; k < std::min(kk + tile_k, A.cols()); k++) {
              dot_prod += A(i,k) * B(k,j);
            }
            C(i,j) = dot_prod;
          }
        }
        /**************************************************************/
      }
    }
  }
}
