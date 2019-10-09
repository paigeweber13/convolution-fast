#include "../convolution/generate_kernels.h"
#include "../convolution/convolution.h"
#include <iostream>

void test_generate_kernels(){
  size_t max_k = 9;
  vector<Kernel> kernels = generate_blur_kernels(max_k);

  for(size_t i = 3; i <= max_k; i += 2){
    cout << kernels[i].to_string();
  }
}

int main(int argc, char* argv[]){
  test_generate_kernels();

  return 0;
}