// third-party
#include <iostream>
#include <string>
#include <chrono>

// mine
#include "convolution.h"

using namespace std;

int main(int argc, char** argv){
  // two possible usages
  string usage1 = string(argv[0]) + " m n k";
  string usage2 = string(argv[0]) + " input_image_data.csv";

  if(argc < 2){
    cout << "usages:" << endl;
    cout << usage1 << endl;
    cout << usage2 << endl;
  } else if(argc < 4){
    auto image = load_image(argv[1]);
    // auto edges = convolve(image, generate_sobel_v_kernel());
    auto edges = convolve(image, generate_blur_kernel(5));
    save_image(edges, "output.csv");
  } else {
    // generate dummy image of size n, m and kernel of size k. Convolve them
    // and report time.
    auto image = generate_garbage_image(stoi(argv[1]), stoi(argv[2]));
    auto kernel = generate_blur_kernel(stoi(argv[3]));

    auto start_time = std::chrono::high_resolution_clock::now();

    auto output = convolve(image, kernel);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    double duration_seconds = duration/10e9;
    cout << to_string(duration_seconds) << endl;
  }

  return 0;
}