#include "../generate_kernels.cpp"

int main(int argc, char* argv[]){
  vector<vector<vector<float>>> kernels = generate_kernels(15);

  for(auto kernel : kernels){
    print_kernel(kernel);
  }

  return 0;
}