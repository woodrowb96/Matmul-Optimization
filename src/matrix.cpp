#include "matrix.h"

#include <vector>
#include <cstdio>
#include <random>
#include <utility>
#include <initializer_list>
#include <stdexcept>

/******************************** PUBLIC ***********************************/

//CONSTRUCTOR
Matrix::Matrix(std::initializer_list<std::initializer_list<float>> rows)
{
  if(rows.size() == 0) {
    throw std::invalid_argument("Matrix: empty initializer list");
  }
  if(rows.begin()->size() == 0) {
    throw std::invalid_argument("Matrix: empty row");
  }

  rows_ = static_cast<int>(rows.size());
  cols_ = static_cast<int>(rows.begin()->size());
  buf_.reserve(rows_ * cols_);

  for(const auto& row : rows) {
    if(static_cast<int>(row.size()) != cols_) {
      throw std::invalid_argument("Matrix: row length mismatch");
    }
    for(float f : row) {
      buf_.push_back(f);
    }
  }
}


//FACTORY METHODS
Matrix Matrix::random(int rows, int cols, unsigned int seed)
{
  std::mt19937 gen(seed);
  std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

  Matrix rand_matrix(rows, cols);
  for(int i = 0; i < (rows * cols); i++) {
      rand_matrix.buf_[i] = dist(gen);
  }
  return rand_matrix;
}

Matrix Matrix::identity(int n)
{
  Matrix identity(n, n);
  for(int i = 0; i < n; i++) {
    identity(i,i) = 1.0f;
  }
  return identity;
}

Matrix Matrix::zeros(int rows, int cols)
{
  return Matrix(rows, cols); //the private constructor returns a rowsXcols matrix filled with zeros
}

//PUBLIC METHODS
void Matrix::print() const
{
  for(int i = 0; i < rows_; i++) {
    for(int j = 0; j < cols_; j++) {
      std::fprintf(stdout, "%8.3f ", buf_[(i * cols_) + j]);
    }
    std::fprintf(stdout, "\n");
  }
}

void Matrix::zero()
{
  std::fill(buf_.begin(), buf_.end(), 0.0f);
}

/********************************** PRIVATE **************************************/

//PRIVATE CONSTRUCTOR
Matrix::Matrix(int rows, int cols)
{
  if(rows <= 0 || cols <= 0) {
    throw std::invalid_argument("Matrix(): invalid matrix size");
  }
  rows_ = rows;
  cols_ = cols;
  buf_.resize(rows * cols);
}

