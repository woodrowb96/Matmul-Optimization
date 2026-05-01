#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <random>

class Matrix
{
  public:
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, std::vector<float> data);

    // static Matrix random(int rows, int cols, unsigned int seed = std::random_device{}());

    void print() const;
  private:
    int rows_ = 0;
    int cols_ = 0;
    std::vector<float> data_;
};

#endif
