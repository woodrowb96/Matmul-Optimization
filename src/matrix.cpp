#include "matrix.h"

#include <vector>
#include <stdexcept>
#include <iostream>

Matrix::Matrix(int rows, int cols)
  :
    rows_(rows),
    cols_(cols),
    data_(rows * cols)
{}

Matrix::Matrix(int rows, int cols, std::vector<float> data)
  :
    rows_(rows),
    cols_(cols),
    data_(data)
{
  if(data_.size() != static_cast<size_t>(rows_ * cols_)) {
    throw std::invalid_argument(
        "Matrix data size " + std::to_string(data_.size()) +
        " does not match dimensions " + std::to_string(rows_) + "x" + std::to_string(cols_));
  }
}

void Matrix::print() const
{
  for(int i = 0; i < rows_; i++) {
    for(int j = 0; j < cols_; j++) {
      std::cout << data_[(i * cols_) + j] << " ";
    }
    std::cout << "\n";
  }
}
