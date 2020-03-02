#include "convolution.h"
#include <iostream>
#include <performance_monitor.h>

void convolve(Image & input_image, Image & output_image, Kernel & kernel){
  size_t k = kernel.get_k();
  size_t m = kernel.get_midpoint();
  size_t width = input_image.get_width();
  size_t height = input_image.get_height();

    /* speedup:
        * "#pragma omp for collapse(2)"
          this linearizes the two for loops - combines these two for lopos
        * tiling will improve locality
        * unrolling loop will yield better performance
          for example:
          sum += image[i-1][j-1]
          sum += image[i-1][j]
          ...
          sum += image[i+1][i+1]
    */

  #pragma omp parallel
  {
    performance_monitor::startRegion("convolution");
    float sum;
    // #pragma omp for collapse(2)
    #pragma omp for
    for(size_t y = BORDER_SIZE; y < height+BORDER_SIZE; y++){
      for(size_t x = BORDER_SIZE; x < width+BORDER_SIZE; x++){
        sum = 0;
        for(size_t n = 0; n < k; n++){
          for(size_t o = 0; o < k; o++){
            sum += input_image.pixels[y-m+n][x-m+o] * kernel.values[n][o];
          }
        }
        output_image.pixels[y][x] = uint8_t(sum);
      }
    }
    performance_monitor::stopRegion("convolution");
  }
}

