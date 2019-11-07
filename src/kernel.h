#pragma once

#include <iomanip>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Kernel {
  public:
    Kernel();
    Kernel(size_t k);
    ~Kernel();
    size_t get_k();
    int get_midpoint();
    vector<vector<float>> values;
    string to_string();

  private:
    size_t k;
    int midpoint;
};
