#include "convolution.h"

vector<vector<uint8_t>> convolve(vector<vector<uint8_t>> image, Kernel kernel){
  auto m = kernel.get_midpoint();
  // create new output image
  auto output_image = vector<vector<uint8_t>>(image);

  #pragma omp parallel
  {
    // don't compute edges
    #pragma omp for
    for(size_t i = m; i < image.size() - m; i++){
      // for each row
      for(size_t j = m; j < image[i].size() - m; j++){
        // for each pixel in that row

      }
    }
  }
}

int main(int argc, char** argv){
  return 0;
}