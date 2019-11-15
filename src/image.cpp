#include "image.h"
#include <iostream>

Image::Image(){
}

Image::Image(size_t m, size_t n){
  allocate_pixel_memory(m, n, false);
}

Image::Image(size_t m, size_t n, bool pinned){
  allocate_pixel_memory(m, n, pinned);
}

Image::~Image(){
  for(size_t i = 0; i < m+2*BORDER_SIZE; i++){
    free(pixels[i]);
  }

  free(pixels);
}

void Image::allocate_pixel_memory(size_t m, size_t n, bool pinned){
  this->m = m;
  this->n = n;

  pixels = (float**)aligned_alloc(ALIGNMENT, sizeof(float*)*(m+BORDER_SIZE*2));

  for(size_t i = 0; i < m+2*BORDER_SIZE; i++){
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

size_t Image::get_m(){ return m; }
size_t Image::get_height(){ return m; }
size_t Image::get_n(){ return n; }
size_t Image::get_width(){ return n; }

bool Image::operator==(Image& other){
  if(this->get_m() != other.get_m() || this->get_n() != other.get_n()){
    return false;
  }

  for(size_t i = 0; i < m; i++){
    for(size_t j = 0; j < n; j++){
      if (*this->at(i, j) != *other.at(i, j)) {
        return false;
      }
    }
  }
  
  return true;
}

bool Image::operator!=(Image& other){ return !(*this == other); }

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
    }
    output << row_output << endl;
  }

  output.close();
}

// m is height, n is length
void Image::randomize(){
  for(size_t i = 0; i < m; i++){
    for(size_t j = 0; j < n; j++){
      *at(i, j) = rand() % 256;
    }
  }
}

