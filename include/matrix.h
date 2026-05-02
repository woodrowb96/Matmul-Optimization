#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <random>

class Matrix
{
  public:
    //constructor
    Matrix(int rows, int cols);

    //factory methods
    static Matrix random(int rows, int cols, unsigned int seed = std::random_device{}());

    //operator overloads
    float& operator()(int i, int j);
    float  operator()(int i, int j) const;

    //public methods
    void print() const;
  private:
    int rows_ = 0;
    int cols_ = 0;
    std::vector<float> data_;
};

#endif
