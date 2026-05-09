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
    Matrix(std::initializer_list<std::initializer_list<float>> rows);

    //factory methods
    static Matrix random(int rows, int cols, unsigned int seed = std::random_device{}());
    static Matrix identity(int n);
    static Matrix zeros(int rows, int cols);

    //operator overloads
    float& operator()(int i, int j) {
      assert(i >= 0 && i < rows_ && j >= 0 && j < cols_);
      return buf_[(i * cols_) + j];
    };
    float  operator()(int i, int j) const {
      assert(i >= 0 && i < rows_ && j >= 0 && j < cols_);
      return buf_[(i * cols_) + j];
    };
    float* data()             {return buf_.data(); };
    const float* data() const {return buf_.data(); };

    //getters
    int rows() const { return rows_; };
    int cols() const { return cols_; };

    //public methods
    void print() const;
    void zero();          //zero (so make all entries 0) an already existing matrix

  private:
    //private constructor
    //  This constructor will make a matrix of size rowsXcols that is all zero.
    //  I am making this private, so the Matrix::zero() factory to get a zero matrix.
    //  This way users use the public constructor to make a matrix filled with custom data,
    //  and use the factories to get matrices filled with "pre-made" data.
    //  This way users need to be explicite about what is in the matrix they are making
    //  (Matrix a = Matrix::zero(1,5) is more explicit than Matrix a(1,5)
    Matrix(int rows, int cols);

    int rows_ = 0;
    int cols_ = 0;
    std::vector<float> buf_;
};

#endif
