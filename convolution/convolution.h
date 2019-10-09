#pragma once

// third party
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

// my stuff
#include "kernel.h"

using namespace std;

vector<vector<uint8_t>> convolve(vector<vector<uint8_t>> image, Kernel kernel);
vector<vector<uint8_t>> load_image(string filename);
void save_image(vector<vector<uint8_t>> image, string filename);
