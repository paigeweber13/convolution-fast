#include "convolution.h"

vector<vector<uint8_t>> convolve(vector<vector<uint8_t>> image, Kernel kernel){
  auto m = kernel.get_midpoint();
  // create new output image
  auto output_image = vector<vector<uint8_t>>(image);

  #pragma omp parallel
  {
    // don't compute edges
    #pragma omp for
    for(size_t i = 0; i < (image.size() - 2*m); i++){
      // for each row
      for(size_t j = 0; j < (image[i].size() - 2*m); j++){
        // for each pixel in that row
        float sum = 0;
        for(size_t k = 0; k < kernel.values.size(); k++){
          for(size_t l = 0; l < kernel.values[k].size(); l++){
            sum += image[i+k][j+l] * kernel.values[k][l];
          }
        }
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
  vector<vector<uint8_t>> image;
  std::size_t current, previous;
  string delim = ",";

  while(getline(input, line)){
    vector<uint8_t> row;
    current = 0;
    previous = 0;
    current = line.find(delim);
    while(current != string::npos){
      row.push_back(stoi(line.substr(previous, current-previous)));
      previous = current + 1;
      current = line.find(delim, previous);
    }
    row.push_back(stoi(line.substr(previous, current-previous)));
    image.push_back(row);
  }

  input.close();

  return image;
}

void save_image(vector<vector<uint8_t>> image, string filename){
  ofstream output;
  output.open(filename);
  string row_output;

  for(auto row : image){
    row_output = "";
    for(auto pixel : row){
      row_output += to_string(pixel) + ",";
    }
    row_output.back() = '\n';
    output << row_output;
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
