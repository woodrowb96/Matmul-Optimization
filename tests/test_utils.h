#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "matrix.h"

#include <stdexcept>
#include <cmath>

inline bool matrices_approx_equal (const Matrix& a, const Matrix& b, float tol = 1e-3)
{
  if(a.rows() != b.rows() || a.cols() != b.cols()) {
    throw std::invalid_argument("matrices_approx_equal(): dimension mismatch");
  }

  for(int i = 0; i < a.rows(); i++) {
    for(int j = 0; j < a.cols(); j++) {
      if( std::abs(a(i,j) - b(i,j)) > tol) {
        return false;
      }
    }
  }
  return true;
}

#endif
