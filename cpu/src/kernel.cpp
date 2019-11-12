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

  for (size_t i = 0; i < k; i++){
    values.push_back(vector<float>(k));
  }

  this->k = k;
  midpoint = int(ceil(k/2));
}

Kernel::~Kernel(){
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
  for(size_t i = 0; i < values.size(); i++){
    for(size_t j = 0; j < values[i].size(); j++){
      ss << fixed << setprecision(4) << values[i][j] << "  ";
    }
    ss << endl;
  }
  ss << endl;
  return ss.str();
}
