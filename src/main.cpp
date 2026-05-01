#include "matrix.h"

#include <iostream>

int main()
{
  Matrix a (5, 3);
  Matrix b (10, 4);
  Matrix c (3, 4, {.3, 2, 1, 5, 6, -52, .222, -.6, .2, 5, 66, 1.5});
  std::cout << "A:\n";
  a.print();
  std::cout << "B:\n";
  b.print();
  std::cout << "C:\n";
  c.print();
  try {
    Matrix d(2,3, {1, 2});
    std::cout << "D:\n";
    d.print();
  }
  catch (const std::invalid_argument& e) {
    std::cout << "caught invalid argument: " << e.what() << "\n";
  }
  return 0;
}
