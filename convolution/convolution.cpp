#include "convolution.h"

#include <iostream>

void blur_3x3(vector<vector<uint8_t>>& image, 
    vector<vector<uint8_t>>& output_image, Kernel kernel){
}

void blur_convolve(vector<vector<uint8_t>>& image, 
    vector<vector<uint8_t>>& output_image, size_t k){
}

void convolve_single_pixel(vector<vector<uint8_t>>& image, 
    vector<vector<uint8_t>>& output_image, size_t i, size_t j, Kernel kernel){
  float sum = 0;
  size_t m = kernel.get_midpoint();
  for(size_t n = 0; n < kernel.get_k(); n++){
    for(size_t o = 0; o < m; o++){
      sum += image[i+n][j+o] * kernel.values[n][o];
    }
  }
  output_image[i+m][j+m] = uint8_t(sum);
}

void convolve(vector<vector<uint8_t>>& image, 
    vector<vector<uint8_t>>& output_image, Kernel kernel){
  auto m = kernel.get_midpoint();
  auto k = kernel.get_k();
  auto width = image[0].size();
  auto height = image.size();

  #pragma omp parallel
  {
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
    #pragma omp for collapse(2)
    for(size_t ymul = 1; ymul < height/Y_TILE_SIZE; ymul++){
      for(size_t xmul = 1; xmul < width/X_TILE_SIZE; xmul++){
        for(size_t i = 0; i < X_TILE_SIZE; i++){
          for(size_t j = 0; j < Y_TILE_SIZE; j++){
            convolve_single_pixel(image, output_image, i*xmul+7, j*ymul+7,
                kernel);
          }
        }
      }
    }
  }
    
    // don't compute edges
    // #pragma omp for
  //   for(size_t i = 0; i < (height - 2*m); i++){
  //     // cout << "we are on row: " << to_string(i) << endl;
  //     // for each row
  //     for(size_t j = 0; j < (width - 2*m); j++){
  //       // for each pixel in that row
  //       float sum = 0;
  //       for(size_t n = 0; n < k; n++){
  //         for(size_t o = 0; o < k; o++){
  //           sum += image[i+n][j+o] * kernel.values[n][o];
  //         }
  //       }
  //       // overflow is not possible because of nature of gaussian blur
  //   
      // cout << "current pixel: " << to_stm] = uint8_t(sum);
  //   
      // cout << "current pixel: " << to_st
  //   
      // cout << "current pixel: " << to_st
  // }
}

vector<vector<uint8_t>> load_image(string filename){
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

  vector<vector<uint8_t>> image(height+14);
  current = 0;
  previous = 0;
  for (size_t i = 7; i < height+7; i++){
    vector<uint8_t> row(width+14);
    for (size_t j = 7; j < width+7; j++){
      current = image_string.find(delim, previous);
      row[j] = stoi(image_string.substr(previous, current-previous));
      previous = current + 1;
    }
    image[i] = row;
  }

  input.close();

  return image;
}

void save_image(vector<vector<uint8_t>> image, string filename){
  ofstream output;
  output.open(filename);
  string row_output;

  auto width = image[7].size();
  auto height = image.size();

  // outputing header
  output << "P2" << endl;
  output << to_string(width-14) << " " << to_string(height-14)
         << endl;
  output << "255" << endl;

  for(size_t i = 7; i < height-7; i++){
    row_output = "";
    for(size_t j = 7; j < width-7; j++){
      row_output += to_string(image[i][j]) + " ";
    }
    output << row_output << endl;
  }

  output.close();
}

// m is length, n is height
vector<vector<uint8_t>> generate_garbage_image(size_t m, size_t n){
  // give it enough margin on both sides
  vector<vector<uint8_t>> result(n+14);
  for(size_t i = 0; i < n; i++){
    result[i] = vector<uint8_t>(m+14);
    for(size_t j = 0; j < m; j++){
      result[i][j] = rand() % 256;
    }
  }
  return result;
}
