#include "convolution_gpu.h"

void convolve_gpu(Image &input, Image &output, Kernel &kernel){
  convolve_gpu(input, output, kernel, input.get_m(), 32);
}

void convolve_gpu(Image &input, Image &output, Kernel &kernel,
    size_t num_blocks, size_t num_threads){
  input.copy_to_gpu();
  output.copy_to_gpu();
  for(size_t i = BORDER_SIZE; i < input.get_n() + BORDER_SIZE; i += num_threads){
    // printf("i: %lu\n", i);
    d_convolve_gpu<<<num_blocks, num_threads>>>(input.d_gpu_pixels, 
        output.d_gpu_pixels, kernel.d_gpu_values, input.get_m(), input.get_n(),
        kernel.get_k(), kernel.get_midpoint(), i);
  }
  output.copy_from_gpu();
}

void __global__ d_convolve_gpu(float** d_input, float** d_output, 
    float** d_kernel, size_t m, size_t n, size_t k, int mid, size_t start_i){
  size_t x = start_i + threadIdx.x + BORDER_SIZE;
  size_t y = blockIdx.x + BORDER_SIZE;
  if(start_i > 7){
    printf("start_i: %lu\n", start_i);
  }
  // printf("x, y: %lu, %lu\n", x, y);
  if(y == 500 && x == 500){
    printf("inside device code\n");
    printf("input 500, 500: %f\n", d_input[500][500]);
  }
  
  if(x < n && y < m){
    float sum = 0;
    for(size_t o = 0; o < k; o++){
      for(size_t p = 0; p < k; p++){
        sum += d_input[y-mid+o][x-mid+p] * d_kernel[o][p];
      }
    }
    d_output[y][x] = uint8_t(sum);
  }

  if(y == 500 && x == 500){
    printf("output 500, 500: %f\n", d_output[500][500]);
  }
}

// copy async data in
// run kernel in stream
// copy async data out

