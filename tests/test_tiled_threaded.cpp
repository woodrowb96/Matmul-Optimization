#include "test_utils.h"
#include "matmul_tiled_threaded.h"

#include <thread>
#include <random>

//We want to test matmul_tiled_threaded with different thread counts
MatmulFn make_tiled_threaded_fn(int num_threads)
{
  return [num_threads] (const Matrix& a, const Matrix& b, Matrix& c,
                        int tile_i, int tile_j, int tile_k) {
    matmul_tiled_threaded(a, b, c, tile_i, tile_j, tile_k, num_threads);
  };
}

int main()
{
  //We can use these ranges to generate random matrix row/col sizes
  std::uniform_int_distribution<int> small(2, 8);
  std::uniform_int_distribution<int> medium(8, 64);
  std::uniform_int_distribution<int> large(64, 1024);

  unsigned int seed {50};
  std::mt19937 gen(seed);

  MatmulFn tiled_threaded_1  = make_tiled_threaded_fn(1);
  MatmulFn tiled_threaded_2  = make_tiled_threaded_fn(2);
  MatmulFn tiled_threaded_4  = make_tiled_threaded_fn(4);
  MatmulFn tiled_threaded_hw = make_tiled_threaded_fn(std::thread::hardware_concurrency());

  int num_fails {0};

  num_fails += run_random_tests(tiled_threaded_4, gen, large,
                                /*runs =*/ 10,
                                /*Tile size =*/ 32, 32, 32,
                                "Trial 1 (4 threads, rand matrix dim, cubed tile(32)):");

  num_fails += run_random_tests(tiled_threaded_1, gen, large,
                                /*runs =*/ 10,
                                /*Tile size =*/ 32, 32, 32,
                                "Trial 2 (1 thread, rand matrix dim, cubed tile(32)):");

  num_fails += run_random_tests(tiled_threaded_2, gen, large,
                                /*runs =*/ 10,
                                /*Tile size =*/ 32, 32, 32,
                                "Trial 3 (2 threads, rand matrix dim, cubed tile(32)):");

  num_fails += run_random_tests(tiled_threaded_hw, gen, large,
                                /*runs =*/ 10,
                                /*Tile size =*/ 32, 32, 32,
                                "Trial 4 (max available threads, rand matrix dim, cubed tile(32)):");

  num_fails += run_random_tests(tiled_threaded_4, gen, large,
                                /*runs =*/ 10,
                                /*tile_size=*/ 16, 16, 16,
                                "Trial 5 (random large matrix with cubed tile(16)):");

  num_fails += run_random_tests(tiled_threaded_4, gen, medium,
                                /*runs =*/ 10,
                                /*tile_size=*/ 16, 16, 16,
                                "Trial 6 (random medium matrix with cubed tile(16)):");

  num_fails += run_random_tests(tiled_threaded_4, gen, large,
                                /*runs =*/ 10,
                                /*tile_size=*/ 53, 5, 7,
                                "Trial 7 (non-cube tile dimensions):");

  num_fails += run_random_tests(tiled_threaded_4, gen, small,
                                /*runs =*/ 10,
                                /*tile_size =*/ 16, 16, 16,
                                "Trial 8 (tile size larger than matrix dimensions):");

  if(!run_test(tiled_threaded_4,
               Matrix::random(33,33,gen()),
               Matrix::random(33,33,gen()),
                /*tile_size =*/ 32, 32, 32,
               "Trial 9 (matrices 1 dimension larger than tile dimensions):")) {
    num_fails++;
  }

  return num_fails != 0;
}
