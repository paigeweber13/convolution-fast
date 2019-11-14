#include <boost/program_options.hpp>
#include <chrono>
#include <iostream>
#include <string>

#include "convolution.h"

using namespace std;
namespace po = boost::program_options;

// function prototypes
void time_single_gpu_test(size_t m, size_t n, size_t k);
double time_single_cpu_test(float** image, float** output, unsigned m,
                            unsigned n, unsigned k);
po::options_description get_program_options_desc();
void test_blur_image(string input_filename, string output_filename);

int main(int argc, char** argv){
  auto desc = get_program_options_desc();

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << desc << endl;
    return 1;
  } 
  else if (vm.count("image-input")) {
    cout << "image-input" << endl;
  }
  else if (vm.count("speedtest")) {
    cout << "speedtest" << endl;
  }

  return 0;
}

void time_single_gpu_test(size_t m, size_t n, size_t k){
  auto input = generate_garbage_image(m, n);
  auto output = generate_garbage_image(m, n);
  Kernel kernel(k);
  kernel.make_blur_kernel();

  convolve(input, output, kernel);

  cout << setw(10) << m << setw(10) << n << setw(10) << k << endl;
}

double time_single_cpu_test(float** image, float** output, unsigned m,
                            unsigned n, unsigned k){
    // Convolve them and report time.
    size_t num_runs = 1;
    Kernel kernel(k);
    kernel.make_blur_kernel();

    auto start_time = chrono::high_resolution_clock::now();

    for(size_t i = 0; i < num_runs; i++){
      convolve(image, output, kernel);
    }

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
    double duration_seconds = duration/(num_runs*1e9);
    return duration_seconds;
}

po::options_description get_program_options_desc(){
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "display this help message")
    ("arch,a", po::value<char>()->default_value('c'),
        "select architecture to use (c for cpu, g for gpu)")
    ("image-input,imi", po::value<string>(), "image to blur convolve")
    ("image-output,imo", po::value<string>()->default_value(
        string("output.pgm")),
        "file to output image (in ascii pgm format). This does nothing "
        "unless image-input is also specified")
    ("speedtest,s", po::value<vector<size_t>>(),
     "m, n, k (m, n are width, height of image and k is size of blur kernel")
  ;

  return desc;
}

void test_blur_image(string input_filename, string output_filename){
  cout << "Testing blur kernel on image " << input_filename << endl;

  cout << "loading image..." << endl;
  auto image = load_image(input_filename);
  // creating output for blurred
  auto blurred = load_image(input_filename);
  cout << "finished loading!" << endl;

  cout << "blurring..." << endl;
  // auto edges = convolve(image, generate_sobel_v_kernel());
  Kernel kernel(5);
  kernel.make_blur_kernel();
  convolve(image, blurred, kernel);
  cout << "finished blurring!" << endl;

  cout << "saving image to " << output_filename << endl;
  save_image(blurred, output_filename);
  cout << "finished saving!" << endl;
}

