#pragma once

#include <iomanip>
#include <iostream>
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
    int get_midpoint();
    float get(int x, int y);
    void set(int x, int y, float value);
    string to_string();

  private:
    vector<vector<float>> values;
    size_t k;
    int midpoint;
};
