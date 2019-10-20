#pragma once

// third party
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

// my stuff
#include "blur_kernels.h"
#include "kernel.h"
#include "generate_kernels.h"

using namespace std;

void convolve(vector<vector<uint8_t>>& image, 
    vector<vector<uint8_t>>& output_image, Kernel kernel);
vector<vector<uint8_t>> load_image(string filename);
void save_image(vector<vector<uint8_t>> image, string filename);
vector<vector<uint8_t>> generate_garbage_image(size_t m, size_t n);
