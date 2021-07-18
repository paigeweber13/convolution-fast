# Fast image convolution in C++
Includes handwritten CPU implementation using OpenMP, handwritten GPU
implementation using CUDA, and implementations using some tools (like
arrayFire)

# Running
first, make sure you have conan installed (`pip install conan`)

    mkdir build
    cd build
    conan install ..
    cd ..
    make

# TODO:
 - try to improve speedup on openGL implementation
 - remove borders on image output
 - write GPU code
 - use CPU code to check GPU code for correctness
 - write convolution with other frameworks and compare speed
   - HPX
   - ArrayFire?
   - C++20 coroutines

