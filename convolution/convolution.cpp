#include "convolution.h"

#include <iostream>

void blur_3x3(
  vector<vector<float, aligned_allocator<float, 32> >>& image, 
  vector<vector<float, aligned_allocator<float, 32> >>& output){
  auto width = image[BORDER_SIZE].size()-BORDER_SIZE*2;
  auto height = image.size()-BORDER_SIZE*2;

  #pragma omp parallel
  {
    // load kernel
    // last five elements in each vector will be garbage, unfortunately
    auto kernel_row_1 = _mm256_load_ps(&blur_kernels[3][0][0]);
    auto kernel_row_2 = _mm256_load_ps(&blur_kernels[3][1][0]);
    auto kernel_row_3 = _mm256_load_ps(&blur_kernels[3][2][0]);

    // place to store results for horizontal add
    vector<float, aligned_allocator<float, 32>> final_vector(8);

    #pragma omp for
    for(size_t y = BORDER_SIZE; y < height+BORDER_SIZE; y ++){
      for(size_t x = BORDER_SIZE; x < width+BORDER_SIZE; x ++){
        auto image_row_1 = _mm256_load_ps(&image[y  ][x]);
        auto image_row_2 = _mm256_load_ps(&image[y+1][x]);
        auto image_row_3 = _mm256_load_ps(&image[y+2][x]);

        // a * b + c
        auto a = _mm256_mul_ps (kernel_row_1, image_row_1);
        auto b = _mm256_fmadd_ps (kernel_row_2, image_row_2, a);
        auto c = _mm256_fmadd_ps (kernel_row_3, image_row_3, b);

        _mm256_store_ps(&final_vector[0], c);

        output[y][x] = uint8_t(
          final_vector[0] + final_vector[1] + final_vector[2]
        );
      }
    }
  }
}

void blur_5x5(
  vector<vector<float, aligned_allocator<float, 32> >>& image, 
  vector<vector<float, aligned_allocator<float, 32> >>& output){
  auto width = image[BORDER_SIZE].size()-BORDER_SIZE*2;
  auto height = image.size()-BORDER_SIZE*2;

  // this code below shows that the memory is sequentially allocated, row-major
  // cout << "5x5 kernel info" << endl;
  // cout << "i, j: value  - memaddress     - addr % 32" << endl;
  // for(size_t i = 0; i < 5; i++){
  //   for(size_t j = 0; j < 8; j++){
  //     cout << to_string(i) << ", " << to_string(j) << ": "
  //          << blur_kernels[5][i][j] << " - " << &blur_kernels[5][i][j]
  //          << " - " << reinterpret_cast<intptr_t>(&blur_kernels[5][i][j]) % 32 
  //          << endl;
  //   }
  //   cout << endl;
  // }

  // however, since the images are huge, a 5x5 part of an image is NOT
  // sequentially allocated. We have to load from a few places in memory...
  // cout << "memory addresses of 5x5 part of image" << endl;
  // for(size_t i = 100; i < 105; i++){
  //   for(size_t j = 100; j < 105; j++){
  //     cout << to_string(i) << ", " << to_string(j) << ": "
  //          << &image[i][j] << endl;
  //   }
  //   cout << endl;
  // }

  #pragma omp parallel
  {
    // load kernel
    // last three elements in each vector will be garbage, unfortunately
    auto kernel_row_1 = _mm256_load_ps(&blur_kernels[5][0][0]);
    auto kernel_row_2 = _mm256_load_ps(&blur_kernels[5][1][0]);
    auto kernel_row_3 = _mm256_load_ps(&blur_kernels[5][2][0]);
    auto kernel_row_4 = _mm256_load_ps(&blur_kernels[5][3][0]);
    auto kernel_row_5 = _mm256_load_ps(&blur_kernels[5][4][0]);

    // place to store results for horizontal add
    vector<float, aligned_allocator<float, 32>> final_vector(8);

    #pragma omp for
    for(size_t y = BORDER_SIZE; y < height+BORDER_SIZE; y ++){
      for(size_t x = BORDER_SIZE; x < width+BORDER_SIZE; x ++){
        auto image_row_1 = _mm256_load_ps(&image[y  ][x]);
        auto image_row_2 = _mm256_load_ps(&image[y+1][x]);
        auto image_row_3 = _mm256_load_ps(&image[y+2][x]);
        auto image_row_4 = _mm256_load_ps(&image[y+3][x]);
        auto image_row_5 = _mm256_load_ps(&image[y+4][x]);

        // a * b + c
        auto a = _mm256_mul_ps (kernel_row_1, image_row_1);
        auto b = _mm256_fmadd_ps (kernel_row_2, image_row_2, a);
        auto c = _mm256_fmadd_ps (kernel_row_3, image_row_3, b);
        auto d = _mm256_fmadd_ps (kernel_row_4, image_row_4, c);
        auto e = _mm256_fmadd_ps (kernel_row_5, image_row_5, d);

        _mm256_store_ps(&final_vector[0], e);

        output[y][x] = uint8_t(
          final_vector[0] + final_vector[1] + final_vector[2] +
          final_vector[3] + final_vector[4] + final_vector[5]
        );
      }
    }
  }
}

void blur_7x7(
  vector<vector<float, aligned_allocator<float, 32> >>& image, 
  vector<vector<float, aligned_allocator<float, 32> >>& output){
  auto width = image[BORDER_SIZE].size()-BORDER_SIZE*2;
  auto height = image.size()-BORDER_SIZE*2;

  #pragma omp parallel
  {
    // load kernel
    auto kernel_row_1 = _mm256_load_ps(&blur_kernels[7][0][0]);
    auto kernel_row_2 = _mm256_load_ps(&blur_kernels[7][1][0]);
    auto kernel_row_3 = _mm256_load_ps(&blur_kernels[7][2][0]);
    auto kernel_row_4 = _mm256_load_ps(&blur_kernels[7][3][0]);
    auto kernel_row_5 = _mm256_load_ps(&blur_kernels[7][4][0]);
    auto kernel_row_6 = _mm256_load_ps(&blur_kernels[7][5][0]);
    auto kernel_row_7 = _mm256_load_ps(&blur_kernels[7][6][0]);

    // place to store results for horizontal add
    vector<float, aligned_allocator<float, 32>> final_vector(8);

    #pragma omp for
    for(size_t y = BORDER_SIZE; y < height+BORDER_SIZE; y ++){
      for(size_t x = BORDER_SIZE; x < width+BORDER_SIZE; x ++){
        auto image_row_1 = _mm256_load_ps(&image[y  ][x]);
        auto image_row_2 = _mm256_load_ps(&image[y+1][x]);
        auto image_row_3 = _mm256_load_ps(&image[y+2][x]);
        auto image_row_4 = _mm256_load_ps(&image[y+3][x]);
        auto image_row_5 = _mm256_load_ps(&image[y+4][x]);
        auto image_row_6 = _mm256_load_ps(&image[y+5][x]);
        auto image_row_7 = _mm256_load_ps(&image[y+6][x]);

        // a * b + c
        auto a = _mm256_mul_ps (kernel_row_1, image_row_1);
        auto b = _mm256_fmadd_ps (kernel_row_2, image_row_2, a);
        auto c = _mm256_fmadd_ps (kernel_row_3, image_row_3, b);
        auto d = _mm256_fmadd_ps (kernel_row_4, image_row_4, c);
        auto e = _mm256_fmadd_ps (kernel_row_5, image_row_5, d);
        auto f = _mm256_fmadd_ps (kernel_row_6, image_row_6, e);
        auto g = _mm256_fmadd_ps (kernel_row_7, image_row_7, f);

        _mm256_store_ps(&final_vector[0], g);

        output[y][x] = uint8_t(
          final_vector[0] + final_vector[1] + final_vector[2] +
          final_vector[3] + final_vector[4] + final_vector[5] +
          final_vector[6] + final_vector[7]
        );
      }
    }
  }
}

void blur_9x9(
  vector<vector<float, aligned_allocator<float, 32> >>& image, 
  vector<vector<float, aligned_allocator<float, 32> >>& output){
  auto width = image[BORDER_SIZE].size()-BORDER_SIZE*2;
  auto height = image.size()-BORDER_SIZE*2;

  #pragma omp parallel
  {
    // load kernel
    auto kernel_row_1 = _mm256_load_ps(&blur_kernels[9][0][0]);
    auto kernel_row_2 = _mm256_load_ps(&blur_kernels[9][1][0]);
    auto kernel_row_3 = _mm256_load_ps(&blur_kernels[9][2][0]);
    auto kernel_row_4 = _mm256_load_ps(&blur_kernels[9][3][0]);
    auto kernel_row_5 = _mm256_load_ps(&blur_kernels[9][4][0]);
    auto kernel_row_6 = _mm256_load_ps(&blur_kernels[9][5][0]);
    auto kernel_row_7 = _mm256_load_ps(&blur_kernels[9][6][0]);
    auto kernel_row_8 = _mm256_load_ps(&blur_kernels[9][7][0]);
    auto kernel_row_9 = _mm256_load_ps(&blur_kernels[9][8][0]);

    // place to store results for horizontal add
    vector<float, aligned_allocator<float, 32>> final_vector(16);

    #pragma omp for
    for(size_t y = BORDER_SIZE; y < height+BORDER_SIZE; y ++){
      for(size_t x = BORDER_SIZE; x < width+BORDER_SIZE; x ++){
        auto image_row_1 = _mm256_load_ps(&image[y-4][x]);
        auto image_row_2 = _mm256_load_ps(&image[y-3][x]);
        auto image_row_3 = _mm256_load_ps(&image[y-2][x]);
        auto image_row_4 = _mm256_load_ps(&image[y-1][x]);
        auto image_row_5 = _mm256_load_ps(&image[y  ][x]);
        auto image_row_6 = _mm256_load_ps(&image[y+1][x]);
        auto image_row_7 = _mm256_load_ps(&image[y+2][x]);
        auto image_row_8 = _mm256_load_ps(&image[y+3][x]);
        auto image_row_9 = _mm256_load_ps(&image[y+4][x]);

        // a * b + c
        auto a = _mm256_mul_ps (kernel_row_1, image_row_1);
        auto b = _mm256_fmadd_ps (kernel_row_2, image_row_2, a);
        auto c = _mm256_fmadd_ps (kernel_row_3, image_row_3, b);
        auto d = _mm256_fmadd_ps (kernel_row_4, image_row_4, c);
        auto e = _mm256_fmadd_ps (kernel_row_5, image_row_5, d);
        auto f = _mm256_fmadd_ps (kernel_row_6, image_row_6, e);
        auto g = _mm256_fmadd_ps (kernel_row_7, image_row_7, f);
        auto h = _mm256_fmadd_ps (kernel_row_8, image_row_8, g);
        auto i = _mm256_fmadd_ps (kernel_row_9, image_row_9, h);

        _mm256_store_ps(&final_vector[0], i);
        float sum_ninth_row = 
          image[y-4][x+8] * blur_kernels[9][0][8] +
          image[y-3][x+8] * blur_kernels[9][1][8] +
          image[y-2][x+8] * blur_kernels[9][2][8] +
          image[y-1][x+8] * blur_kernels[9][3][8] +
          image[y  ][x+8] * blur_kernels[9][4][8] +
          image[y+1][x+8] * blur_kernels[9][5][8] +
          image[y+2][x+8] * blur_kernels[9][6][8] +
          image[y+3][x+8] * blur_kernels[9][7][8] +
          image[y+4][x+8] * blur_kernels[9][8][8];


        output[y][x] = uint8_t(
          final_vector[0] + final_vector[1] + final_vector[2] +
          final_vector[3] + final_vector[4] + final_vector[5] +
          final_vector[6] + final_vector[7] + final_vector[8] +
          sum_ninth_row
        );
      }
    }
  }
}

void blur_convolve(
  vector<vector<float, aligned_allocator<float, 32> >>& image, 
  vector<vector<float, aligned_allocator<float, 32> >>& output, 
  size_t k){
  switch(k) {
    case 3:
      blur_3x3(image, output);
      break;
    case 5:
      blur_5x5(image, output);
      break;
    case 7:
      blur_7x7(image, output);
      break;
    case 9:
      blur_9x9(image, output);
      break;
    case 11:
      // blur_11x11(image, output);
      break;
    case 13:
      // blur_13x13(image, output);
      break;
    case 15:
      // blur_15x15(image, output);
      break;
    default:
      throw runtime_error(
        "k must be an odd number between 3 and 15, inclusive");
      break;
  }
}

void convolve(
    vector<vector<float, aligned_allocator<float, 32> >>& image, 
    vector<vector<float, aligned_allocator<float, 32> >>& output_image, 
    Kernel kernel){
  auto width = image[BORDER_SIZE].size()-BORDER_SIZE*2;
  auto height = image.size()-BORDER_SIZE*2;
  float sum = 0;
  size_t k = kernel.get_k();
  size_t m = kernel.get_midpoint();

    /* speedup:
        * "#pragma omp for collapse(2)"
          this linearizes the two for loops - combines these two for lopos
        * tiling will improve locality
        * unrolling loop will yield better performance
          for example:
          sum += image[i-1][j-1]
          sum += image[i-1][j]
          ...
          sum += image[i+1][i+1]
    */

  #pragma omp parallel
  {
    // #pragma omp for collapse(2)
    #pragma omp for
    for(size_t y = BORDER_SIZE; y < height+BORDER_SIZE; y++){
      for(size_t x = BORDER_SIZE; x < width+BORDER_SIZE; x++){
        sum = 0;
        for(size_t n = 0; n < k; n++){
          for(size_t o = 0; o < k; o++){
            sum += image[y-m+o][x-m+n] * kernel.values[n][o];
          }
        }
        output_image[y][x] = uint8_t(sum);
      }
    }
  }
}

vector<vector<float, aligned_allocator<float, 32> >> 
    load_image(string filename){
  ifstream input;
  input.open(filename);
  string line;
  std::size_t current, previous;
  string delim = " ";

  // skip first line, we just support P2
  getline(input, line);

  // second line has width and height
  getline(input, line);
  auto i = line.find(delim);
  size_t width = stoi(line.substr(0, i));
  auto j = line.find(delim);
  size_t height = stoi(line.substr(i+1, j));

  // skip third line, we just support 255 as max value
  getline(input, line);

  string image_string;
  while(getline(input, line)){
    image_string += line;
  }

  vector<vector<float, aligned_allocator<float, 32> >>
    image(height+BORDER_SIZE*2);
  current = 0;
  previous = 0;
  // initialize borders to 0
  // top
  for (size_t i = 0; i < BORDER_SIZE; i++){
    image[i] = vector<float, aligned_allocator<float, 32>>(
      width+BORDER_SIZE*2);
  }
  // bottom
  for (size_t i = image.size()-1; i >= image.size()-BORDER_SIZE; i--){
    image[i] = vector<float, aligned_allocator<float, 32>>(
      width+BORDER_SIZE*2);
  }

  for (size_t i = BORDER_SIZE; i < height+BORDER_SIZE; i++){
    vector<float, aligned_allocator<float, 32>> row(width+BORDER_SIZE*2);
    for (size_t j = BORDER_SIZE; j < width+BORDER_SIZE; j++){
      current = image_string.find(delim, previous);
      row[j] = stoi(image_string.substr(previous, current-previous));
      previous = current + 1;
    }
    image[i] = row;
  }

  input.close();

  return image;
}

void save_image(
    vector<vector<float, aligned_allocator<float, 32> >> image,
    string filename){
  ofstream output;
  output.open(filename);
  string row_output;

  auto width = image[BORDER_SIZE].size();
  auto height = image.size();

  // outputing header
  output << "P2" << endl;
  output << to_string(width-BORDER_SIZE*2) << " "
         << to_string(height-BORDER_SIZE*2) << endl;
  output << "255" << endl;

  for(size_t i = BORDER_SIZE; i < height-BORDER_SIZE; i++){
    row_output = "";
    for(size_t j = BORDER_SIZE; j < width-BORDER_SIZE; j++){
      row_output += to_string(uint8_t(image[i][j])) + " ";
    }
    output << row_output << endl;
  }

  output.close();
}

// m is length, n is height
vector<vector<float, aligned_allocator<float, 32> >>
    generate_garbage_image(size_t m, size_t n){
  // give it enough margin on both sides
  vector<vector<float, aligned_allocator<float, 32> >>
    result(n+BORDER_SIZE*2);
  for(size_t i = 0; i < n+BORDER_SIZE*2; i++){
    result[i] = vector<float, aligned_allocator<float, 32>>(
                  m+BORDER_SIZE*2);
    for(size_t j = 0; j < m+BORDER_SIZE*2; j++){
      result[i][j] = rand() % 256;
    }
  }
  return result;
}
