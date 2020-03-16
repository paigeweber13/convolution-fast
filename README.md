# Fast image convolution in C++
Includes handwritten CPU implementation using OpenMP, handwritten GPU
implementation using CUDA, and implementations using some tools (like
arrayFire)

# NOTES FOR FINE HARDWARE VISUALIZATION:
 - both `test_blur_image` (can be run with `make test`) and `speedtest` (can be
   run with `./convolution.out -s 1000 1000 15`) should be instrumented

# Running
first, make sure you have conan installed (`pip install conan`)

    mkdir build
    cd build
    conan install ..
    cd ..
    make

# TODO:
 - image output has borders
 - write GPU code
 - use CPU code to check GPU code for correctness

