#include "image.h"
#include <iostream>

Image::Image(){
}

Image::Image(size_t m, size_t n){
  // cout << "allocating..." << endl;
  // cout << "m: " << m << " n: " << n << endl;
  // cout << "m+BORDER_SIZE*2: " << m+BORDER_SIZE*2 << endl;
  this->m = m;
  this->n = n;

  pixels = (float**)aligned_alloc(ALIGNMENT, sizeof(float*)*(m+BORDER_SIZE*2));

  for(size_t i = 0; i < m+2*BORDER_SIZE; i++){
    // cout << "allocating row " << i << endl;
    pixels[i] = (float*)aligned_alloc(ALIGNMENT, sizeof(float)*(n+BORDER_SIZE*2));

    if (i < BORDER_SIZE || i >= m){
      for(size_t j = 0; j < n+2*BORDER_SIZE; j++){
        pixels[i][j] = BORDER_VALUE;
      }
    }
    else {
      for(size_t j = 0; j < BORDER_SIZE; j++){
        pixels[i][j] = BORDER_VALUE;
      }
      for(size_t j = n; j < n+BORDER_SIZE; j++){
        pixels[i][j] = BORDER_VALUE;
      }
    }
  }
}

Image::~Image(){
  // cout << "freeing..." << endl;
  // cout << "m: " << m << " n: " << n << endl;
  // cout << "m+BORDER_SIZE*2: " << m+BORDER_SIZE*2 << endl;

  for(size_t i = 0; i < m+2*BORDER_SIZE; i++){
    // cout << "freeing row " << i << endl;
    free(pixels[i]);
  }

  free(pixels);
}

size_t Image::get_m(){ return m; }
size_t Image::get_height(){ return m; }
size_t Image::get_n(){ return n; }
size_t Image::get_width(){ return n; }

float* Image::at(size_t i, size_t j){
  return &pixels[i+BORDER_SIZE][j+BORDER_SIZE];
}

Image Image::load_image(string filename){
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

  Image image(height, width);
  current = 0;
  previous = 0;

  for (size_t i = 0; i < height; i++){
    for (size_t j = 0; j < width; j++){
      current = image_string.find(delim, previous);
      (*image.at(i,j)) = stoi(image_string.substr(previous, current-previous));
      previous = current + 1;
    }
  }

  input.close();

  return image;
}

void Image::save_image(Image &image, string filename){
  ofstream output;
  output.open(filename);
  string row_output;

  // outputing header
  output << "P2" << endl;
  // n is width, m is height
  output << to_string(image.get_n()) << " "
         << to_string(image.get_m()) << endl;
  output << "255" << endl;

  for(size_t i = 0; i < image.get_m(); i++){
    row_output = "";
    for(size_t j = 0; j < image.get_n(); j++){
      row_output += to_string(uint8_t(*image.at(i,j))) + " ";
      // if (*image.at(i, j) == 0 && (i < 10 || j < 10)){
      //   printf("zero at %lu, %lu\n", i, j);
      // }
    }
    output << row_output << endl;
  }

  output.close();
}

// m is height, n is length
void Image::randomize(){
  // cout << "randomizing..." << endl;
  // cout << "m, n: " << m << ", " << n << endl;
  for(size_t i = 0; i < m; i++){
    for(size_t j = 0; j < n; j++){
      // cout << "i, j: " << i << ", " << j << endl;
      *at(i, j) = rand() % 256;
    }
  }
}

