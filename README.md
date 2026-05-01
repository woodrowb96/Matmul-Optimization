# Matmul Optimization

The goal of this project is to go from a naive matmul implementation to an optimized tiling
matmul implementation running on the GPU. Along the way I will benchmark each optimization
and keep track of the results as more optimization is added. I will also use the benchmark
data to tune the tiling matmul parameters (tile size and depth) to my computers cache size,
so that we get optimal performance.

The planned optimization path is this:

Naive -> tiled (single-threaded) -> tiled (multi-thread) -> tiled (CUDA)
