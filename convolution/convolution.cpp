#include "convolution.h"

#include <iostream>

vector<vector<uint8_t>> convolve(vector<vector<uint8_t>> image, Kernel kernel){
  auto m = kernel.get_midpoint();
  auto k = kernel.get_k();
  // create new output image
  auto output_image = vector<vector<uint8_t>>(image);

  #pragma omp parallel
  {
    // don't compute edges
    // "#pragma omp for collapse(2)"
    // this linearizes the two for loops - combines these two for lopos
    // tiling will improve locality
    #pragma omp for
    for(size_t i = 0; i < (image.size() - 2*m); i++){
      // cout << "we are on row: " << to_string(i) << endl;
      // for each row
      for(size_t j = 0; j < (image[i].size() - 2*m); j++){
        // for each pixel in that row
        float sum = 0;
        // unrolling loop will yield better performance
        for(size_t n = 0; n < k; n++){
          for(size_t o = 0; o < k; o++){
            sum += image[i+n][j+o] * kernel.values[n][o];
          }
        }
        // overflow is possible because of rounding errors....
        // clamp to prevent overflow
        sum = min(sum, 255.0f);
        output_image[i+m][j+m] = uint8_t(sum);
      }
    }
  }

  return output_image;
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

  vector<vector<uint8_t>> image(height);
  current = 0;
  previous = 0;
  for (size_t i = 0; i < height; i++){
    vector<uint8_t> row(width);
    current = image_string.find(delim, previous);
    for (size_t j = 0; j < width; j++){
      row[j] = stoi(image_string.substr(previous, current-previous));
      previous = current + 1;
      current = image_string.find(delim, previous);
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

  auto width = image[0].size();
  auto height = image.size();

  // outputing header
  output << "P2" << endl;
  output << to_string(width) << " " << to_string(height)
         << endl;
  output << "255" << endl;

  for(auto row : image){
    row_output = "";
    for(auto pixel : row){
      row_output += to_string(pixel) + " ";
    }
    output << row_output << endl;
  }

  output.close();
}

// m is length, n is height
vector<vector<uint8_t>> generate_garbage_image(size_t m, size_t n){
  vector<vector<uint8_t>> result(n);
  for(size_t i = 0; i < n; i++){
    result[i] = vector<uint8_t>(m);
    for(size_t j = 0; j < m; j++){
      result[i][j] = rand() % 256;
    }
  }
  return result;
}
