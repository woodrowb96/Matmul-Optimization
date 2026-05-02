#include "matrix.h"

#include <iostream>

int main()
{
  Matrix a (5, 3);
  Matrix b (10, 4);
  Matrix c = Matrix::random(7, 2);
  Matrix d = Matrix::random(3, 3, 42);
  Matrix e (3, 3, {21.0f, -3.0f, 0.3f, 0.3f, 66.0f, 212.0f, 9.0f, 5.0f, 6.0f});
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

  a(1,1) = 5.0f;
  a(0,0) = 3.3f;
  std::cout << "A:\n";
  a.print();
  std::cout << "A[1,2]: " << a(1,2) << "\n";

  std::cout << "C.rows(): " << c.rows() << "\n";
  std::cout << "C.cols(): " << c.cols() << "\n";

  d.zero();
  std::cout << "D:\n";
  d.print();

  return 0;
}
