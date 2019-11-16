#pragma once

#include <iomanip>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

#define ALIGNMENT 32

using namespace std;

class Kernel {
  public:
    Kernel();
    Kernel(size_t k);
    ~Kernel();

    size_t get_k();
    int get_midpoint();

    bool operator==(Kernel& other);
    bool operator!=(Kernel& other);

    float ** values;
    cudaArray_t gpu_values;

    string to_string();
    void make_blur_kernel();
    void copy_to_gpu();

    // only the elements at odd indices from 3 to max_k inclusive have valid kernel
    // objects. The rest contain garbage data.
    
    // YOU MUST MANUALLY DESTROY THESE KERNELS. ex:
    // Kernel* = new k(3);
    // k->~Kernel();
    static vector<Kernel> generate_blur_kernels(size_t max_k);

  private:
    size_t k;
    int midpoint;
    bool is_on_gpu;
    static float bivariate_gaussian(float std_dev, float x, float y);
    static Kernel generate_blur_kernel(size_t k);
};

