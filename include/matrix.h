#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <random>

class Matrix
{
  public:
    //constructor
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, std::vector<float> data);

    //factory methods
    static Matrix random(int rows, int cols, unsigned int seed = std::random_device{}());

    //operator overloads
    float& operator()(int i, int j);
    float  operator()(int i, int j) const;

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
