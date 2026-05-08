#include "matmul_tiled_gpu.h"

#include <cstdio>

__global__ void hello_kernal()
{
  printf("hello from GPU thread %d\n", threadIdx.x);
}

void hello_world_gpu()
{
  hello_kernal<<<2, 8>>>();
  cudaDeviceSynchronize();
}
