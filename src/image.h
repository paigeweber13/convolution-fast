#pragma once

#include <cstdint>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>

#define ALIGNMENT 32
#define BORDER_SIZE 7
#define BORDER_VALUE 122

using namespace std;

class Image {
  public:
    // statics
    static Image load_image(string filename);
    static void save_image(Image &image, string filename);

    // constructors/destructors
    Image();
    Image(size_t m, size_t n);
    Image(size_t m, size_t n, bool pinned);
    ~Image();

    // member utility functions
    float* at(size_t i, size_t j);
    float** pixels;
    float** gpu_pixels;
    float** d_gpu_pixels;
    void randomize();
    void copy_to_gpu();

    // operators
    bool operator==(Image& other);
    bool operator!=(Image& other);

    // getters
    size_t get_m(); //height of image
    size_t get_n(); //width of image
    size_t get_height(); // equivalent to get_m()
    size_t get_width(); // equivalent to get_n()

  private:
    // m = width, n = height
    size_t m;
    size_t n;

    void allocate_pixel_memory(size_t m, size_t n, bool pinned);

    bool is_on_gpu;
    bool is_pinned;
};

