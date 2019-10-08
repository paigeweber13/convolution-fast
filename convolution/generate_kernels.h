#pragma once

#include <iostream>
#include <math.h>
#include <vector>

#include "kernel.h"

#define _USE_MATH_DEFINES

float bivariate_gaussian(float std_dev, float x, float y);
Kernel generate_blur_kernel(size_t k);
// only the elements at odd indices from 3 to max_k inclusive have valid kernel
// objects. The rest contain garbage data.
vector<Kernel> generate_blur_kernels(size_t max_k);
