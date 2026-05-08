#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "matrix.h"
#include "matmul_naive.h"

#include <stdexcept>
#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <random>

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

using MatmulFn = std::function<void(const Matrix&, const Matrix&, Matrix&, int, int, int)>;

inline bool run_test(MatmulFn matmul,
                    const Matrix& left, const Matrix& right,
                    int tile_i, int tile_j, int tile_k,
                    const std::string& label)
{
  //Use matmul naive to calculate the expected result
  Matrix expected = Matrix::zeros(left.rows(), right.cols());
  matmul_naive(left, right, expected);

  //Use the matmul we are testing to calc the actual result
  Matrix actual   = Matrix::zeros(left.rows(), right.cols());
  matmul(left, right, actual, tile_i, tile_j, tile_k);

  //Handle Test pass/fail
  if(!matrices_approx_equal(actual, expected)) {
    std::cerr << "Test Fail " << label        << ": ";
    std::cerr << "tile: "     << tile_i       << "x" << tile_j       << "x" << tile_k << " ";
    std::cerr << "left: "     << left.rows()  << "x" << left.cols()  << " ";
    std::cerr << "right: "    << right.rows() << "x" << right.cols() << "\n";
    return false;
  }
  return true;
}

inline int run_random_tests(MatmulFn matmul,
                            std::mt19937& gen,
                            std::uniform_int_distribution<int> size_dist,
                            int num_runs,
                            int tile_i, int tile_j, int tile_k,
                            const std::string& label)
{
  int num_fails {0};

  for(int i = 0; i < num_runs; i++) {
    //Randomize matrix sizes
    int left_rows  = size_dist(gen);
    int left_cols  = size_dist(gen);
    int right_rows = left_cols;       //inner dimensions need to match
    int right_cols = size_dist(gen);

    //Randomize the seed, used to generate random  values in the matrices
    unsigned int left_seed = gen();
    unsigned int right_seed = gen();

    std::string run_label = label +
                            "Run: "        + std::to_string(i)         + " " +
                            "left_seed: "  + std::to_string(left_seed) + " " +
                            "right_seed: " + std::to_string(right_seed);

    bool test_passed = run_test(matmul,
                                Matrix::random(left_rows,  left_cols,  left_seed),
                                Matrix::random(right_rows, right_cols, right_seed),
                                tile_i, tile_j, tile_k,
                                run_label);

    if(!test_passed) num_fails++;
  }
  return num_fails;
}
#endif
