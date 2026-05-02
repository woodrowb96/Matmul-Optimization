#include "matmul_naive.h"
#include "matrix.h"

#include <cassert>

void matmul_naive(const Matrix& A, const Matrix& B, Matrix& C)
{
  assert(A.cols() == B.rows());
  assert((C.rows() == A.rows()) && (C.cols() == B.cols()));

  for(int i = 0; i < A.rows(); i++) {
    for(int j = 0; j < B.cols(); j++) {
      float dot_prod = 0.0f;
      for(int k = 0; k < A.cols(); k++) {
        dot_prod += A(i,k) * B(k,j);
      }
      C(i,j) = dot_prod;
    }
  }
}
