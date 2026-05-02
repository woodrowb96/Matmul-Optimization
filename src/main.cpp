#include "matrix.h"

#include <iostream>

int main()
{
  Matrix a (5, 3);
  Matrix b (10, 4);
  Matrix c = Matrix::random(7, 2);
  Matrix d = Matrix::random(3, 3, 42);
  Matrix e (3, 3, {21, -3, .3, .3, 66, 212, 9, 5, 6});
  std::cout << "A:\n";
  a.print();
  std::cout << "B:\n";
  b.print();
  std::cout << "C:\n";
  c.print();
  std::cout << "D:\n";
  d.print();
  std::cout << "e:\n";
  e.print();

  a(1,1) = 5;
  a(0,0) = 3.3;
  std::cout << "A:\n";
  a.print();
  std::cout << "A[1,2]: " << a(1,2) << "\n";
  return 0;
}
