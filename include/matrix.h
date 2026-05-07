#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <initializer_list>
#include <random>
#include <cassert>

class Matrix
{
  public:
    //constructor
    Matrix(int rows, int cols);
    Matrix(std::initializer_list<std::initializer_list<float>> rows);

    //factory methods
    static Matrix random(int rows, int cols, unsigned int seed = std::random_device{}());

    //operator overloads
    float& operator()(int i, int j) {
      assert(i >= 0 && i < rows_ && j >= 0 && j < cols_);
      return data_[(i * cols_) + j];
    };
    float  operator()(int i, int j) const {
      assert(i >= 0 && i < rows_ && j >= 0 && j < cols_);
      return data_[(i * cols_) + j];
    };

    //getters
    int rows() const { return rows_; };
    int cols() const { return cols_; };

    //public methods
    void print() const;
    void zero();

  private:
    int rows_ = 0;
    int cols_ = 0;
    std::vector<float> data_;
};

#endif
