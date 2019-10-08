#include "generate_kernels.h"

using namespace std;

// a kernel is just a vector<vector<float>>
// elements are accessed with kernel[row][column]

float bivariate_gaussian(float std_dev, float x, float y){
  return 1/(2*M_PI*pow(std_dev, 2)) * 
    exp(-(pow(x, 2) + pow(y, 2))/(2*pow(std_dev,2)));
}

Kernel generate_blur_kernel(size_t k){
  Kernel result(k);
  // will need tuning
  // float std_dev = float(k) * 2/3;
  float std_dev = 1.0;

  int half = ceil(k/2);
  float sum = 0.0;
  for(int x = -half; x <= half; x++){
    for(int y = -half; y <= half; y++){
      result.set(x ,y, bivariate_gaussian(std_dev, x, y));
      sum += result.get(x, y);
    }
  }

  // normalize!
  for(int x = -half; x <= half; x++){
    for(int y = -half; y <= half; y++){
      result.set(x, y, result.get(x, y)/sum);
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
