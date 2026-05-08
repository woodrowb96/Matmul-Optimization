#include "test_utils.h"
#include "matmul_naive.h"
#include "matmul_tiled_cpu.h"

#include <iostream>
#include <random>
#include <string>

bool run_test(const Matrix& left, const Matrix& right,
              int tile_i, int tile_j, int tile_k,
              const std::string& label)
{
  //Use matmul naive to calculate the expected result
  Matrix expected = Matrix::zeros(left.rows(), right.cols());
  matmul_naive(left, right, expected);

  //Use matmul_tiled to calc the actual result
  Matrix actual   = Matrix::zeros(left.rows(), right.cols());
  matmul_tiled_cpu(left, right, actual, tile_i, tile_j, tile_k);

  //Handle Test pass/fail
  if(!matrices_approx_equal(actual, expected)) {
    std::cerr << "Test Fail " << label        << ": ";
    std::cerr << "tile: "     << tile_i       << "x" << tile_j       << "x" << tile_k << " ";
    std::cerr << "left: "     << left.rows()  << "x" << left.cols()  << " ";
    std::cerr << "right: "    << right.rows() << "x" << right.cols() << "\n";
    return false;
  }
  return true;
}

int run_random_tests(std::mt19937& gen,
                    std::uniform_int_distribution<int> size_dist,
                    int num_runs,
                    int tile_i, int tile_j, int tile_k,
                    const std::string& label)
{
  int num_fails {0};

  for(int i = 0; i < num_runs; i++) {
    //Randomize matrix sizes
    int left_rows  = size_dist(gen);
    int left_cols  = size_dist(gen);
    int right_rows = left_cols;       //inner dimensions need to match
    int right_cols = size_dist(gen);

    //Randomize the seed, used to generate random  values in the matrices
    unsigned int left_seed = gen();
    unsigned int right_seed = gen();

    std::string run_label = label +
                            "Run: "        + std::to_string(i)         + " " +
                            "left_seed: "  + std::to_string(left_seed) + " " +
                            "right_seed: " + std::to_string(right_seed);

    bool test_passed = run_test(Matrix::random(left_rows,  left_cols,  left_seed),
                                Matrix::random(right_rows, right_cols, right_seed),
                                tile_i, tile_j, tile_k,
                                run_label);

    if(!test_passed) num_fails++;
  }
  return num_fails;
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

  num_fails += run_random_tests(gen,
                                large,
                                10,
                                32, 32, 32,
                                "Trial 1 (random large matrix with cubed tile(32)):");

  num_fails += run_random_tests(gen,
                                large,
                                10,
                                16, 16, 16,
                                "Trial 2 (random large matrix with cubed tile(16)):");

  num_fails += run_random_tests(gen,
                                medium,
                                10,
                                16, 16, 16,
                                "Trial 3 (random medium matrix with cubed tile(16)):");

  num_fails += run_random_tests(gen,
                                large,
                                10,
                                64, 5, 7,
                                "Trial 4 (non-cube tile dimensions):");

  num_fails += run_random_tests(gen,
                                small,
                                10,
                                16, 16, 16,
                                "Trial 5 (tile size larger than matrix dimensions):");

  if(!run_test(Matrix::random(33,33,gen()),
               Matrix::random(33,33,gen()),
               32, 32, 32,
               "Trial 6 (matrices 1 dimension larger than tile dimensions):")) {
    num_fails++;
  }

  return num_fails != 0;
}
