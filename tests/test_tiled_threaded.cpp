#include "test_utils.h"
#include "matmul_tiled_threaded.h"

#include <thread>
#include <random>

MatmulFn make_tiled_threaded_fn(int num_threads, int tile_i, int tile_j, int tile_k)
{
  return [num_threads, tile_i, tile_j, tile_k] (const Matrix& a, const Matrix& b, Matrix& c) {
    matmul_tiled_threaded(a, b, c, tile_i, tile_j, tile_k, num_threads);
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
  int hw_threads = std::thread::hardware_concurrency();

  num_fails += run_random_tests(make_tiled_threaded_fn(4, 32, 32, 32),
                                gen, large, 10,
                                "Trial 1 (4 threads, rand matrix dim, cubed tile(32)):");

  num_fails += run_random_tests(make_tiled_threaded_fn(1, 32, 32, 32),
                                gen, large, 10,
                                "Trial 2 (1 thread, rand matrix dim, cubed tile(32)):");

  num_fails += run_random_tests(make_tiled_threaded_fn(2, 32, 32, 32),
                                gen, large, 10,
                                "Trial 3 (2 threads, rand matrix dim, cubed tile(32)):");

  num_fails += run_random_tests(make_tiled_threaded_fn(hw_threads, 32, 32, 32),
                                gen, large, 10,
                                "Trial 4 (max available threads, rand matrix dim, cubed tile(32)):");

  num_fails += run_random_tests(make_tiled_threaded_fn(4, 16, 16, 16),
                                gen, large, 10,
                                "Trial 5 (random large matrix with cubed tile(16)):");

  num_fails += run_random_tests(make_tiled_threaded_fn(4, 16, 16, 16),
                                gen, medium, 10,
                                "Trial 6 (random medium matrix with cubed tile(16)):");

  num_fails += run_random_tests(make_tiled_threaded_fn(4, 53, 5, 7),
                                gen, large, 10,
                                "Trial 7 (non-cube tile dimensions):");

  num_fails += run_random_tests(make_tiled_threaded_fn(4, 16, 16, 16),
                                gen, small, 10,
                                "Trial 8 (tile size larger than matrix dimensions):");

  if(!run_test(make_tiled_threaded_fn(4, 32, 32, 32),
               Matrix::random(33,33,gen()),
               Matrix::random(33,33,gen()),
               "Trial 9 (matrices 1 dimension larger than tile dimensions):")) {
    num_fails++;
  }

  return num_fails != 0;
}
