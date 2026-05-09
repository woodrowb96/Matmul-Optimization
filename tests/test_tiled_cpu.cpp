#include "test_utils.h"
#include "matmul_naive.h"
#include "matmul_tiled_cpu.h"

MatmulFn make_tiled_cpu_fn(int tile_i, int tile_j, int tile_k)
{
  return [tile_i, tile_j, tile_k] (const Matrix& a, const Matrix& b, Matrix& c) {
    matmul_tiled_cpu(a, b, c, tile_i, tile_j, tile_k);
  };
}

int main()
{
  int num_fails {0};

  unsigned int seed {50};
  std::mt19937 gen(seed);

  //We can use these ranges to generate random matrix row/col sizes
  std::uniform_int_distribution<int> small(2, 8);
  std::uniform_int_distribution<int> medium(8, 64);
  std::uniform_int_distribution<int> large(64, 1024);

  num_fails += run_random_tests(make_tiled_cpu_fn(32, 32, 32),
                                gen, large, 10,
                                "Trial 1 (random large matrix with cubed tile(32)):");

  num_fails += run_random_tests(make_tiled_cpu_fn(16, 16, 16),
                                gen, large, 10,
                                "Trial 2 (random large matrix with cubed tile(16)):");

  num_fails += run_random_tests(make_tiled_cpu_fn(16, 16, 16),
                                gen, medium, 10,
                                "Trial 3 (random medium matrix with cubed tile(16)):");

  num_fails += run_random_tests(make_tiled_cpu_fn(64, 5, 7),
                                gen, large, 10,
                                "Trial 4 (non-cube tile dimensions):");

  num_fails += run_random_tests(make_tiled_cpu_fn(16, 16, 16),
                                gen, small, 10,
                                "Trial 5 (tile size larger than matrix dimensions):");

  if(!run_test(make_tiled_cpu_fn(32, 32, 32),
               Matrix::random(33,33,gen()),
               Matrix::random(33,33,gen()),
               "Trial 6 (matrices 1 dimension larger than tile dimensions):")) {
    num_fails++;
  }

  return num_fails != 0;
}
