#include "test_utils.h"
#include "matmul_naive.h"

#include <iostream>

bool test(const Matrix& left, const Matrix& right, const Matrix& expected)
{
  Matrix actual = Matrix::zeros(left.rows(), right.cols());
  matmul_naive(left, right, actual);
  return matrices_approx_equal(actual, expected);
}

int main()
{
  int num_fails = 0;

  // Test 1: test with square matrices
  if (!test(
        {{ 1.0f, 3.0f},
         {-4.0f, 6.0f}},                  //left
        {{5.0f, 3.0f},
         {3.0f, 0.0f}},                   //right
        {{14.0f,  3.0f},
         {-2.0f, -12.0f}}                 //expected
      )) {
    std::cerr << "test_1 (2x2 integer) fail\n";
    num_fails++;
  }

  // Test 2: test with non-square matrices
  if (!test(
        {{ 1.0f, 2.0f, 3.0f},
         {-1.0f, 0.5f, 4.0f}},            //left
        {{2.0f,  0.0f},
         {1.0f,  3.0f},
         {0.0f, -2.0f}},                  //right
        {{ 4.0f,  0.0f},
         {-1.5f, -6.5f}}                  //expected
      )) {
    std::cerr << "test_2 (non-square) fail\n";
    num_fails++;
  }

  // Test 3: test with some float decimal values
  if (!test(
        {{ 1.0f,  2.5f},
         {-3.52f, 5.0f}},                 //left
        {{0.0f,   3.0f},
         {-4.52f, 9.3f}},                 //right
        {{-11.3f, 26.25f},
         {-22.6f, 35.94f}}                //expected
      )) {
    std::cerr << "test_3 (float decimals) fail\n";
    num_fails++;
  }

  // Test 4: matmul with identity matrix
  if (!test(
        Matrix::random(125, 332, /*seed =*/ 100),
        Matrix::identity(332),
        Matrix::random(125, 332, /*seed =*/100)
      )) {
    std::cerr << "test_4 (identity) fail\n";
    num_fails++;
  }

  // Test 5: matmul with zero
  if (!test(
        Matrix::random(333, 99, /*seed =*/ 120),
        Matrix::zeros(99, 500),
        Matrix::zeros(333, 500)
      )) {
    std::cerr << "test_5 (zero) fail\n";
    num_fails++;
  }

  return num_fails != 0;
}
