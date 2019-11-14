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
#include "image.h"
#include "kernel.h"

// size of margin/padding/border/whatever-you-call-it on each side of actual 
// image

using namespace std;

void convolve(Image input_image, Image output_image, Kernel kernel);

