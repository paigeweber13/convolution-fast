#include "../convolution/generate_kernels.h"
#include <iostream>

int main(int argc, char* argv[]){
  size_t max_k = 9;
  vector<Kernel> kernels = generate_blur_kernels(max_k);

  for(size_t i = 3; i <= max_k; i += 2){
    cout << kernels[i].to_string();
  }

  return 0;
}