#include "../generate_kernels.cpp"

int main(int argc, char* argv[]){
  vector<vector<vector<float>>> kernels = generate_kernels(9);

  for(auto kernel : kernels){
    print_kernel(kernel);
  }

  return 0;
}