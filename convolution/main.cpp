// third-party
#include <iostream>
#include <string>
#include <chrono>

// mine
#include "convolution.h"

using namespace std;

double time_single_test(vector<vector<uint8_t>> image, unsigned m, unsigned n,
                        unsigned k){
    // generate dummy image of size m, n and kernel of size k. Convolve them
    // and report time.
    // auto image = generate_garbage_image(m, n);
    auto kernel = generate_blur_kernel(k);

    auto start_time = chrono::high_resolution_clock::now();

    auto output = convolve(image, kernel);

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
    double duration_seconds = duration/10e9;
    return duration_seconds;
}

void test_and_output_various_sizes(){

  unsigned image_sizes[5][2] = {
      {1024, 768},
      {2048, 2048},
      {8192, 8192},
      {4194304, 768},
      {16777216, 768}
  };

  unsigned kernel_sizes[7] = {3, 5, 7, 9, 11, 13, 15};

  // m is length, n is height
  cout << "m,n,k,time (s)" << endl;
  // maximum size for all images. Reuse image between tests

  for (auto image_size : image_sizes){
    auto m = image_size[0];
    auto n = image_size[1];
    auto image = generate_garbage_image(m, n);
    for (auto kernel_size : kernel_sizes){
      cout << to_string(m) << "," << to_string(n) << ","
           << to_string(kernel_size) << ","
           << to_string(time_single_test(image, m,n,kernel_size)) << endl;
    }
  }
}

int main(int argc, char** argv){
  // two possible usages
  string usage1 = string(argv[0]);
  string usage2 = string(argv[0]) + " input_image_data.csv";

  if(argc > 1){
    auto image = load_image(argv[1]);
    // auto edges = convolve(image, generate_sobel_v_kernel());
    auto edges = convolve(image, generate_blur_kernel(5));
    save_image(edges, "output.csv");
  } else {
    test_and_output_various_sizes();
  }

  return 0;
}