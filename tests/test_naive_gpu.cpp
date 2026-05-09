#include "test_utils.h"
#include "matmul_naive_gpu.h"

#include <random>

MatmulFn make_naive_gpu_fn()
{
  return [] (const Matrix& a, const Matrix& b, Matrix& c) {
    matmul_naive_gpu(a, b, c);
  };
}

int main()
{
  std::uniform_int_distribution<int> small(2, 8);
  std::uniform_int_distribution<int> medium(8, 64);
  std::uniform_int_distribution<int> large(64, 1024);

  unsigned int seed {50};
  std::mt19937 gen(seed);

  int num_fails {0};

  num_fails += run_random_tests(make_naive_gpu_fn(), gen, small, 10,
                                "Trial 1 (random small matrices):");

  num_fails += run_random_tests(make_naive_gpu_fn(), gen, medium, 10,
                                "Trial 2 (random medium matrices):");

  num_fails += run_random_tests(make_naive_gpu_fn(), gen, large, 10,
                                "Trial 3 (random large matrices):");

  return num_fails != 0;
}
