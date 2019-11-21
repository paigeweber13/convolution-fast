#include "image.h"
#include "kernel.h"

void convolve_gpu(Image &input, Image &output, Kernel &kernel);
void convolve_gpu(Image &input, Image &output, Kernel &kernel,
    size_t num_blocks, size_t num_threads);
void __global__ d_convolve_gpu(float** d_input, float** d_output, 
    float** d_kernel, size_t m, size_t n, size_t k);

