#include "convolution.h"

void convolve(Image input_image, Image output_image, Kernel kernel){
  float sum = 0;
  size_t k = kernel.get_k();
  size_t m = kernel.get_midpoint();

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
    // #pragma omp for collapse(2)
    #pragma omp for
    for(size_t y = 0; y < input_image.get_m(); y++){
      for(size_t x = 0; x < input_image.get_n(); x++){
        sum = 0;
        for(size_t n = 0; n < k; n++){
          for(size_t o = 0; o < k; o++){
            sum += *input_image.at(y-m+o, x-m+n) * kernel.values[n][o];
          }
        }
        *output_image.at(y, x) = uint8_t(sum);
      }
    }
  }
}

