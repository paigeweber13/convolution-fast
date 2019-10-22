#pragma once

// third party
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <immintrin.h>
#include <stdexcept>
#include <string>
#include <vector>

// my stuff
#include "blur_kernels.h"
#include "kernel.h"
#include "generate_kernels.h"

// size of margin/padding/border/whatever-you-call-it on each side of actual 
// image
#define BORDER_SIZE 14

using namespace std;
using namespace boost::alignment;

void blur_convolve(
  vector<vector<float, aligned_allocator<float>>,
    aligned_allocator<vector<float>>>& image, 
  vector<vector<float, aligned_allocator<float>>,
    aligned_allocator<vector<float>>>& output, 
    size_t k);

void convolve(vector<vector<uint8_t>>& image, 
    vector<vector<uint8_t>>& output_image, Kernel kernel);

vector<vector<float, aligned_allocator<float>>,
    aligned_allocator<vector<float>>> load_image(string filename);

void save_image(vector<vector<float, aligned_allocator<float>>,
    aligned_allocator<vector<float>>> image, string filename);

vector<vector<float, aligned_allocator<float>>,
    aligned_allocator<vector<float>>> 
generate_garbage_image(size_t m, size_t n);
