#include "generate_kernels.h"

using namespace std;

// a kernel is just a vector<vector<float>>
// elements are accessed with kernel[row][column]

float bivariate_gaussian(float std_dev, float x, float y){
  return 1/(2*M_PI*pow(std_dev, 2)) * 
    exp(-(pow(x, 2) + pow(y, 2))/(2*pow(std_dev,2)));
}

Kernel generate_sobel_v_kernel(){
  Kernel result(3);

  // doesn't seem to work?
  result.values[0][0] = -1;
  result.values[0][1] =  0;
  result.values[0][2] =  1;

  result.values[1][0] = -2;
  result.values[1][1] =  0;
  result.values[1][2] =  2;

  result.values[2][0] = -1;
  result.values[2][1] =  0;
  result.values[2][2] =  1;

  return result;
}

Kernel generate_blur_kernel(size_t k){
  Kernel result(k);
  // will need tuning
  // float std_dev = float(k) * 2/3;
  float std_dev = 2.0;

  float sum = 0.0;
  for(int x = -result.get_midpoint(); x <= result.get_midpoint(); x++){
    for(int y = -result.get_midpoint(); y <= result.get_midpoint(); y++){
      auto current_value = bivariate_gaussian(std_dev, x, y);
      result.values[x+result.get_midpoint()][y+result.get_midpoint()]
        = current_value;
      sum += current_value;
    }
  }

  // normalize!
  for(size_t i = 0; i < result.values.size(); i++){
    for(size_t j = 0; j < result.values[i].size(); j++){
      result.values[i][j] /= sum;
    }
  }

  return result;
}

vector<Kernel> generate_blur_kernels(size_t max_k){
  vector<Kernel> result(max_k+1);
  for(size_t i = 3; i <= max_k; i += 2){
    result[i] = generate_blur_kernel(i);
  }
  return result;
}
