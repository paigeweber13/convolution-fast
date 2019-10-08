#pragma once

// third party
#include <cstdint>
#include <vector>

// my stuff
#include "kernel.h"

using namespace std;

vector<vector<uint8_t>> convolve(vector<vector<uint8_t>> image, Kernel kernel);
