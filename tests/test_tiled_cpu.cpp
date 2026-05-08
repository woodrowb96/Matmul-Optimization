#include "test_utils.h"
#include "matmul_naive.h"
#include "matmul_tiled_cpu.h"

int main()
{
  MatmulFn tiled_cpu = matmul_tiled_cpu;

  int num_fails {0};

  unsigned int seed {50};
  std::mt19937 gen(seed);

  //We can use these ranges to generate random matrix row/col sizes
  std::uniform_int_distribution<int> small(2, 8);
  std::uniform_int_distribution<int> medium(8, 64);
  std::uniform_int_distribution<int> large(64, 1024);

  num_fails += run_random_tests(tiled_cpu,
                                gen,
                                large,
                                10,
                                32, 32, 32,
                                "Trial 1 (random large matrix with cubed tile(32)):");

  num_fails += run_random_tests(tiled_cpu,
                                gen,
                                large,
                                10,
                                16, 16, 16,
                                "Trial 2 (random large matrix with cubed tile(16)):");

  num_fails += run_random_tests(tiled_cpu,
                                gen,
                                medium,
                                10,
                                16, 16, 16,
                                "Trial 3 (random medium matrix with cubed tile(16)):");

  num_fails += run_random_tests(tiled_cpu,
                                gen,
                                large,
                                10,
                                64, 5, 7,
                                "Trial 4 (non-cube tile dimensions):");

  num_fails += run_random_tests(tiled_cpu,
                                gen,
                                small,
                                10,
                                16, 16, 16,
                                "Trial 5 (tile size larger than matrix dimensions):");

  if(!run_test(tiled_cpu,
               Matrix::random(33,33,gen()),
               Matrix::random(33,33,gen()),
               32, 32, 32,
               "Trial 6 (matrices 1 dimension larger than tile dimensions):")) {
    num_fails++;
  }

  return num_fails != 0;
}
