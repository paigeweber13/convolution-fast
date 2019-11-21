#include "convolution_gpu.h"

void convolve_gpu(Image input, Image output, Kernel kernel){
  convolve_gpu(input, output, kernel, input.get_m(), input.get_n());
}

void convolve_gpu(Image input, Image output, Kernel kernel, size_t num_blocks,
    size_t num_threads){
  input.copy_to_gpu();
  output.copy_to_gpu();
  d_convolve_gpu<<<num_blocks, num_threads>>>(input.d_gpu_pixels, 
      output.d_gpu_pixels, kernel.d_gpu_values, input.get_m(), input.get_n(),
      kernel.get_k());
  output.copy_from_gpu();
}

void __global__ d_convolve_gpu(float** d_input, float** d_output, 
    float** d_kernel, size_t m, size_t n, size_t k){
  size_t y = blockIdx.x;
  size_t x = threadIdx.x;
  
  float sum = 0;
  for(size_t o = 0; o < k; o++){
    for(size_t p = 0; p < k; p++){
      sum += d_input[y-m+o][x-m+p] * d_kernel[o][p];
    }
  }
  d_output[y][x] = uint8_t(sum);
}

// copy async data in
// run kernel in stream
// copy async data out

