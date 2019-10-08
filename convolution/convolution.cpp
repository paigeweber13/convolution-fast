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
        float sum = 0;
        for(int k = -m; k <= m; k++){
          for(int l = -m; l <= m; l++){
            sum += image[i+k][j+k] * kernel.get(k, l);
          }
        }
        output_image[i][j] = uint8_t(sum);
      }
    }
  }

  return output_image;
}

int main(int argc, char** argv){
  // two possible usages
  auto usage1 = argv[0] + " n m k";
  auto usage2 = argv[0] + " input_image_data.csv";

  if(argc < )
  return 0;
}