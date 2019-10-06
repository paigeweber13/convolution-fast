#include <iomanip>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

// a kernel is just a vector<vector<float>>
// elements are accessed with kernel[row][column]

float bivariate_gaussian(float std_dev, float x, float y){
  return exp(-(pow(x, 2) + pow(y, 2))/(2*pow(std_dev,2)));
}

vector<vector<float>> generate_kernel(size_t k){
  cout << "Gerneating single kernel!" << endl;
  vector<vector<float>> result;
  // will need tuning
  float std_dev = float(k) * 2/3;

  unsigned half = ceil(k/2);
  for(size_t row = 0; row < k; row++){
    result.push_back(vector<float>(k));
    for(size_t column = 0; column < k; column++){
      cout << "row: " << row << " column: " << column << endl;
      result[row][column] = bivariate_gaussian(std_dev, column-half, row-half);
    }
  }
  return result;
}

vector<vector<vector<float>>> generate_kernels(size_t max_k){
  cout << "Gerneating a bunch of kernels!" << endl;
  vector<vector<vector<float>>> result(max_k+1);
  for(size_t i = 1; i <= max_k; i += 2){
    result[i] = generate_kernel(i);
  }
  return result;
}

void print_kernel(vector<vector<float>> kernel){
  cout << "printing single kernel" << endl;
  setprecision(4);
  for(auto row : kernel){
    for(auto element : row){
      cout << element << "  ";
    }
    cout << endl;
  }
  cout << endl;
}
