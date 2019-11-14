#include "kernel.h"

Kernel::Kernel(){
  Kernel(3);
}

Kernel::Kernel(size_t k){
  if (k < 3){
    throw "Unusable k. K must be at least 3.";
  } else if (k % 2 == 0){
    throw "Unusable k. K must be odd.";
  } 

  values = (float **)aligned_alloc(ALIGNMENT, k*sizeof(float *));

  for (size_t i = 0; i < k; i++){
    values[i] = (float *)aligned_alloc(ALIGNMENT, k*sizeof(float));
  }

  this->k = k;
  midpoint = int(ceil(k/2));
}

Kernel::~Kernel(){
  for (size_t i = 0; i < k; i++){
    free(values[i]);
  }

  free(values);
}

int Kernel::get_midpoint(){
  return midpoint;
}

size_t Kernel::get_k(){
  return k;
}

string Kernel::to_string(){
  stringstream ss;
  ss << "k: " << k << endl;
  ss << "Midpoint: " << midpoint << endl;
  ss << "Kernel:" << endl;
  for(size_t i = 0; i < k; i++){
    for(size_t j = 0; j < k; j++){
      ss << fixed << setprecision(4) << values[i][j] << "  ";
    }
    ss << endl;
  }
  ss << endl;
  return ss.str();
}

float bivariate_gaussian(float std_dev, float x, float y){
  return 1/(2*M_PI*pow(std_dev, 2)) * 
    exp(-(pow(x, 2) + pow(y, 2))/(2*pow(std_dev,2)));
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
  for(size_t i = 0; i < result.get_k(); i++){
    for(size_t j = 0; j < result.get_k(); j++){
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

