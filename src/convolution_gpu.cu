#include "convolution_gpu.h"

void convolve_gpu(Image &input, Image &output, Kernel &kernel){
  convolve_gpu(input, output, kernel, input.get_m(), input.get_n());
}

void convolve_gpu(Image &input, Image &output, Kernel &kernel,
    size_t num_blocks, size_t num_threads){
  printf("copying input...\n");
  input.copy_to_gpu();
  printf("copying output...\n");
  output.copy_to_gpu();
  printf("convolving...\n");
  d_convolve_gpu<<<num_blocks, num_threads>>>(input.d_gpu_pixels, 
      output.d_gpu_pixels, kernel.d_gpu_values, input.get_m(), input.get_n(),
      kernel.get_k());
  printf("copying back result...\n");
  output.copy_from_gpu();
}

void __global__ d_convolve_gpu(float** d_input, float** d_output, 
    float** d_kernel, size_t m, size_t n, size_t k){
  size_t y = blockIdx.x + BORDER_SIZE;
  size_t x = threadIdx.x + BORDER_SIZE;
  
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

