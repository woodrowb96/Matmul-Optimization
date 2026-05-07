#include "matrix.h"

#include <vector>
#include <cstdio>
#include <random>
#include <cassert>
#include <utility>
#include <initializer_list>
#include <stdexcept>

/******************************** PUBLIC ***********************************/

//CONSTRUCTOR
Matrix::Matrix(int rows, int cols)
  :
    rows_(rows),
    cols_(cols),
    data_(rows * cols)
{}

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
  data_.reserve(rows_ * cols_);

  for(const auto& row : rows) {
    if(static_cast<int>(row.size()) != cols_) {
      throw std::invalid_argument("Matrix: row length mismatch");
    }
    for(float f : row) {
      data_.push_back(f);
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
      rand_matrix.data_[i] = dist(gen);
  }
  return rand_matrix;
}


//OPERATOR OVERLOADS
// float& Matrix::operator()(int i, int j)
// {
//   assert(i >= 0 && i < rows_ && j >= 0 && j < cols_);
//   return data_[(i * cols_) + j];
// }
//
// float Matrix::operator()(int i, int j) const
// {
//   assert(i >= 0 && i < rows_ && j >= 0 && j < cols_);
//   return data_[(i * cols_) + j];
// }


//PUBLIC METHODS
void Matrix::print() const
{
  for(int i = 0; i < rows_; i++) {
    for(int j = 0; j < cols_; j++) {
      std::fprintf(stdout, "%8.3f ", data_[(i * cols_) + j]);
    }
    std::fprintf(stdout, "\n");
  }
}

void Matrix::zero()
{
  std::fill(data_.begin(), data_.end(), 0.0f);
}
