#include <iostream>

#include "../convolution/convolution.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "usage: " << argv[0] << " width" << endl;
  }

  auto image = generate_garbage_image(stoi(argv[1]), stoi(argv[1]));
  save_image(image, "test.pgm");
}