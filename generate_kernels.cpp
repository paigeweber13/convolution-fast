#include <math.h>
#include <vector>

using namespace std;

float bivariate_gaussian(float std_dev, float x, float y){
  return exp(-(pow(x, 2) + pow(y, 2))/(2*pow(std_dev,2)));
}

vector<vector<float>> generate_kernel(size_t k){
  vector<vector<float>> result;
  // will need tuning
  float std_dev = k * 2/3;
  for(size_t i = 0; i < k; i++){
    for(size_t j = 0; j < k; j++){
      result[i][j] = bivariate_gaussian(std_dev, i, j);
    }
  }
  return result;
}

vector<vector<vector<float>>> generate_kernels(size_t max_k){
  vector<vector<vector<float>>> result;
  for(size_t i = 1; i <= k; i += 2){
    result[i] = generate_kernel(i);
  }
  return result;
}
