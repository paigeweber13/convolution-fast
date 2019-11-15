#include <boost/program_options.hpp>
#include <chrono>
#include <getopt.h>
#include <iostream>
#include <string>

#include "convolution.h"
#include "image.h"
#include "kernel.h"

using namespace std;
namespace po = boost::program_options;

// function prototypes
void print_help();
void time_single_gpu_test(size_t m, size_t n, size_t k);
double time_single_cpu_test(unsigned m, unsigned n, unsigned k);
void test_blur_image(string input_filename, string output_filename, char arch);

int main(int argc, char** argv){
  // time_single_cpu_test(10, 10, 3);
  test_blur_image("tests/saturn-v-2048x2048-bw.pgm", "tests/saturn-v-out.pgm", 'c');
  return 0;

//   if (argc < 2){
//     print_help();
//   }
// 
//   const char* const short_opts = "ha:i:o::s";
//   const option long_opts[] = {
//     {"help", no_argument, nullptr, 'h'},
//     {"arch", required_argument, nullptr, 'a'},
//     {"image-input", required_argument, nullptr, 'i'},
//     {"image-output", required_argument, nullptr, 'o'},
//     // 3 arguments should be provided, they will be deferred to the end
//     {"speedtest", no_argument, nullptr, 's'}, 
//   };
// 
//   char arch = 'c'; 
//   string image_input = "";
//   string image_output = "output.pgm";
//   bool speedtest = false;
// 
//   auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
//   while (opt != -1) {
//     switch (opt){
//       case 'a':
//         arch = *optarg;
//         break;
//       case 'i':
//         image_input = optarg;
//         break;
//       case 'o':
//         image_output = optarg;
//         break;
//       case 's':
//         speedtest = true;
//         break;
// 
//       case 'h':
//       default:
//         print_help();
//         break;
//     }
//     opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
//   }
// 
//   if (speedtest) {
//     auto m = stoul(argv[optind]);
//     auto n = stoul(argv[optind+1]);
//     auto k = stoul(argv[optind+2]);
// 
//     switch (arch) {
//       case 'c':
//         time_single_cpu_test(m, n, k);
//         break;
//       case 'g':
//         // time_single_gpu_test(m, n, k);
//         break;
//       default:
//         cout << "invalid architecture specified!" << endl;
//         return 2;
//     }
//   }
//   else {
//     test_blur_image(image_input, image_output, arch);
//   }
// 
//   return 0;
}

void print_help(){
  std::cout <<
  "  -h, --help:                    Show help\n"
  "  -a, --arch <char>:             indicate if code should be run on cpu \n"
  "                                 (c) or gpu (g)\n"
  "  -i, --image-input <filename>:  image to blur\n"
  "  -o, --image-output <filename>: place to save image (has no effect\n"
  "                                 unless --image-input is also specified\n"
  "                                 note: for both image-input and \n"
  "                                 image-output, only ascii .pgm files are \n"
  "                                 supported.\n"
  "  -s, --speedtest <m> <n> <k>:   time a test with given image size m, n\n"
                                    "and kernel size k\n";
  exit(1);
}

void time_single_gpu_test(size_t m, size_t n, size_t k){
  auto input = Image(m, n);
  input.randomize();
  auto output = Image(m, n);
  Kernel kernel(k);
  kernel.make_blur_kernel();

  convolve(input, output, kernel);

  cout << setw(10) << m << setw(10) << n << setw(10) << k << endl;
}

double time_single_cpu_test(unsigned m, unsigned n, unsigned k){
  // Convolve them and report time.
  size_t num_runs = 1;
  auto input = Image(m, n);
  input.randomize();
  auto output = Image(m, n);
  Kernel kernel(k);
  kernel.make_blur_kernel();

  auto start_time = chrono::high_resolution_clock::now();

  for(size_t i = 0; i < num_runs; i++){
    convolve(input, output, kernel);
  }

  auto end_time = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
  double duration_seconds = duration/(num_runs*1e9);
  return duration_seconds;
}

void test_blur_image(string input_filename, string output_filename, char arch){
  cout << "Testing blur kernel on image " << input_filename << endl;

  cout << "loading image..." << endl;
  auto image = Image::load_image(input_filename);
  // creating output for blurred
  auto blurred = Image(image.get_m(), image.get_n());
  cout << "finished loading!" << endl;

  cout << "blurring..." << endl;
  // auto edges = convolve(image, generate_sobel_v_kernel());
  Kernel kernel(5);
  kernel.make_blur_kernel();
  switch(arch){
    case 'c':
      convolve(image, blurred, kernel);
      break;
    case 'g':
      // convolve_gpu(image, blurred, kernel);
      break;
  }
  cout << "finished blurring!" << endl;
}

