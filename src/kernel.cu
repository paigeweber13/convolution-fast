#include "kernel.h"

#include <iostream>

Kernel::Kernel(){
  Kernel(3);
}

Kernel::Kernel(size_t k){
  if (k < 3){
    throw "Unusable k. K must be at least 3.";
  } else if (k % 2 == 0){
    throw "Unusable k. K must be odd.";
  } 

  // cout << "allocating kernel..." << endl;
  values = (float **)aligned_alloc(ALIGNMENT, k*sizeof(float *));
  is_on_gpu = false;

  for (size_t i = 0; i < k; i++){
    // cout << "allocating kernel row " << i << endl;
    values[i] = (float *)aligned_alloc(ALIGNMENT, k*sizeof(float));
  }

  this->k = k;
  midpoint = int(ceil(k/2));
  // cout << "k: " << this->k << endl;
  // cout << "midpoint: " << this->midpoint << endl;
}

Kernel::~Kernel(){
  // cout << "freeing kernel..." << endl;
  for (size_t i = 0; i < k; i++){
    // printf("freeing kernel row %lu\n", i);
    free(values[i]);
  }

  // printf("freeing rest of kernel \n");
  free(values);

  if (is_on_gpu) {
    for (size_t i = 0; i < k; i++){
      cudaFree(gpu_values[i]);
    }

    cudaFree(d_gpu_values);
    free(gpu_values);
  }
}

int Kernel::get_midpoint(){ return midpoint; }
size_t Kernel::get_k(){ return k; }

bool Kernel::operator==(Kernel& other){
  if(this->get_k() != other.get_k()){
    return false;
  }

  for(size_t i = 0; i < k; i++){
    for(size_t j = 0; j < k; j++){
      if (this->values[i][j] != other.values[i][j]) {
        return false;
      }
    }
  }
  
  return true;
}

bool Kernel::operator!=(Kernel& other){ return !(*this == other); }

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

float Kernel::bivariate_gaussian(float std_dev, float x, float y){
  return 1/(2*M_PI*pow(std_dev, 2)) * 
    exp(-(pow(x, 2) + pow(y, 2))/(2*pow(std_dev,2)));
}

void Kernel::make_blur_kernel(){
  // will need tuning
  // float std_dev = float(k) * 2/3;
  float std_dev = 2.0;

  float sum = 0.0;
  for(int x = -midpoint; x <= midpoint; x++){
    for(int y = -midpoint; y <= midpoint; y++){
      auto current_value = bivariate_gaussian(std_dev, x, y);
      values[x+midpoint][y+midpoint] = current_value;
      sum += current_value;
    }
  }

  // normalize!
  for(size_t i = 0; i < get_k(); i++){
    for(size_t j = 0; j < get_k(); j++){
      values[i][j] /= sum;
    }
  }
}

void Kernel::copy_to_gpu(){
  is_on_gpu = true;

  vector<cudaError_t> errors;

  gpu_values = (float**)aligned_alloc(ALIGNMENT, sizeof(float*)*k);

  for (size_t i = 0; i < k; i++){
    errors.push_back(cudaMalloc(&gpu_values[i], sizeof(float)*k));
    errors.push_back(cudaMemcpy(gpu_values[i], values[i], sizeof(float)*k,
                                cudaMemcpyDefault));
  }

  errors.push_back(cudaMalloc(&d_gpu_values, sizeof(float*)*k));
  errors.push_back(cudaMemcpy(d_gpu_values, gpu_values, sizeof(float*)*k,
                              cudaMemcpyDefault));

  for (size_t i = 0; i < errors.size(); i++){
    if(errors[i] != 0){
      cerr << "error number " << i << ": " 
           << cudaGetErrorName(errors[i]) << endl
           << cudaGetErrorString(errors[i]) << endl;
    }
  }
}

Kernel Kernel::generate_blur_kernel(size_t k){
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

vector<Kernel> Kernel::generate_blur_kernels(size_t max_k){
  vector<Kernel> result(max_k+1);
  for(size_t i = 3; i <= max_k; i += 2){
    result[i] = generate_blur_kernel(i);
  }
  return result;
}

