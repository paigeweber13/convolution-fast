#include "convolution.h"

#include <iostream>

void blur_3x3(
  vector<vector<float, aligned_allocator<float, 32> >>& image, 
  vector<vector<float, aligned_allocator<float, 32> >>& output){
  auto width = image[BORDER_SIZE].size()-BORDER_SIZE*2;
  auto height = image.size()-BORDER_SIZE*2;
  #pragma omp parallel
  {
    #pragma omp for collapse(2)
    for(size_t y = BORDER_SIZE; y < height+BORDER_SIZE; y++){
      for(size_t x = BORDER_SIZE; x < width+BORDER_SIZE; x++){
        ;
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

  // #pragma omp parallel
  // {
    // load kernel
    // last three elements in each vector will be garbage, unfortunately
    auto kernel_row_1 = _mm256_load_ps(&blur_kernels[5][0][0]);
    auto kernel_row_2 = _mm256_load_ps(&blur_kernels[5][1][0]);
    auto kernel_row_3 = _mm256_load_ps(&blur_kernels[5][2][0]);
    auto kernel_row_4 = _mm256_load_ps(&blur_kernels[5][3][0]);
    auto kernel_row_5 = _mm256_load_ps(&blur_kernels[5][4][0]);
    cout << "finished loading entire kernel!\n";

    // place to store results for horizontal add
    vector<float, aligned_allocator<float, 32>> final_vector(8);

    // #pragma omp for collapse(2)
    #pragma omp for
    for(size_t y = BORDER_SIZE; y < height+BORDER_SIZE; y += 5){
      cout << "row number: " + to_string(y) + "\n";
      for(size_t x = BORDER_SIZE; x < width+BORDER_SIZE; x += 5){
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

        // replace the rest of the contents of this loop with hadd because
        // we'll padd the remaining parts of kenerl with zeroes
        _mm256_store_ps(&final_vector[0], e);

        output[y][x] = uint8_t(
          final_vector[0] + final_vector[1] + final_vector[2] +
          final_vector[3] + final_vector[4] + final_vector[5]
        );
      }
    }
  // }
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
      break;
    case 9:
      break;
    case 11:
      break;
    case 13:
      break;
    case 15:
      break;
    default:
      throw runtime_error(
        "k must be an odd number between 3 and 15, inclusive");
      break;
  }
}

void convolve(vector<vector<uint8_t>>& image, 
    vector<vector<uint8_t>>& output_image, Kernel kernel){
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

    // tiling
    #define X_TILE_SIZE 256
    #define Y_TILE_SIZE 64
    // tiles are assigned per processor
  #pragma omp parallel
  {
    #pragma omp for collapse(2)
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
