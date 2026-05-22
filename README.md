# Matmul Optimization

Implementing and benchmarking different optimizations of matrix multiplication in C++/CUDA.

## Optimizations

I have implemented the following versions of matmul. Each iteration is an optimization of the
preceding ones.

1. **Naive (CPU)** — Naive matmul running on the CPU.
2. **Tiled (CPU, single-threaded)** — Tiled matmul running single-threaded on the CPU.
3. **Tiled (CPU, multi-threaded)** — Tiled matmul running multi-threaded on the CPU.
4. **Naive (CUDA)** — Naive matmul running on the GPU.
5. **Tiled (CUDA, shared memory)** — cooperative tile loads into shared memory.
6. **Tiled (CUDA, register tiling)** — each thread computes a REG_M × REG_N output block held in registers, reusing each shared-memory load REG_M (or REG_N) times for higher arithmetic intensity.

## Results

```
Using 12 threads
------------------------------------------------------------------------
Matrix Sizes: (5x5) X (5x7) = (5x7)
Type: naive_cpu                    Min:    0.0002ms Max:    0.0003ms Avg:    0.0002ms
Type: tiled_cpu_single_threaded    Min:    0.0002ms Max:    0.0003ms Avg:    0.0003ms
Type: tiled_cpu_multi_threaded     Min:    0.5813ms Max:    1.0018ms Avg:    0.8129ms
Type: naive_gpu                    Min:    0.1225ms Max:    0.1395ms Avg:    0.1289ms
Type: tiled_gpu_shared_mem         Min:    0.1224ms Max:    0.1647ms Avg:    0.1350ms
Type: tiled_gpu_register           Min:    0.1244ms Max:    0.1315ms Avg:    0.1269ms
------------------------------------------------------------------------
Matrix Sizes: (100x500) X (500x100) = (100x100)
Type: naive_cpu                    Min:    5.0288ms Max:    5.8922ms Avg:    5.3492ms
Type: tiled_cpu_single_threaded    Min:    3.0548ms Max:    3.5559ms Avg:    3.2430ms
Type: tiled_cpu_multi_threaded     Min:    1.4528ms Max:    2.0100ms Avg:    1.6306ms
Type: naive_gpu                    Min:    0.2525ms Max:    0.3265ms Avg:    0.2763ms
Type: tiled_gpu_shared_mem         Min:    0.2487ms Max:    0.2859ms Avg:    0.2597ms
Type: tiled_gpu_register           Min:    0.2621ms Max:    0.3175ms Avg:    0.2861ms
------------------------------------------------------------------------
Matrix Sizes: (1000x1025) X (1025x900) = (1000x900)
Type: naive_cpu                    Min:  943.7743ms Max: 1046.6999ms Avg:  974.7325ms
Type: tiled_cpu_single_threaded    Min:  607.2152ms Max:  617.0223ms Avg:  610.6853ms
Type: tiled_cpu_multi_threaded     Min:  121.0771ms Max:  139.2103ms Avg:  132.1639ms
Type: naive_gpu                    Min:   11.4782ms Max:   15.5346ms Avg:   12.4278ms
Type: tiled_gpu_shared_mem         Min:    9.5557ms Max:   13.6099ms Avg:   11.6110ms
Type: tiled_gpu_register           Min:    3.7079ms Max:    7.8424ms Avg:    5.7879ms
------------------------------------------------------------------------
Matrix Sizes: (2048x1587) X (1587x1024) = (2048x1024)
Type: naive_cpu                    Min: 8021.3514ms Max: 8729.1948ms Avg: 8378.8008ms
Type: tiled_cpu_single_threaded    Min: 3205.5491ms Max: 3454.2801ms Avg: 3333.3713ms
Type: tiled_cpu_multi_threaded     Min:  701.4080ms Max:  755.1588ms Avg:  718.4511ms
Type: naive_gpu                    Min:   40.9060ms Max:   42.3161ms Avg:   41.4112ms
Type: tiled_gpu_shared_mem         Min:   22.5489ms Max:   26.7894ms Avg:   24.5192ms
Type: tiled_gpu_register           Min:    7.7542ms Max:   13.5247ms Avg:    9.9521ms
```

On the largest tested case (2048×1587 × 1587×1024), the register-tiled CUDA kernel runs ~840×
faster than the naive CPU baseline.

## Build & run

```
cmake -S . -B build
cmake --build build
./build/matmul    # benchmarks
```
