#include "convolution.h"

vector<vector<uint8_t>> convolve(vector<vector<uint8_t>> image, Kernel kernel){
  auto m = kernel.get_midpoint();
  // create new output image
  auto output_image = vector<vector<uint8_t>>(image);

  #pragma omp parallel
  {
    // don't compute edges
    #pragma omp for
    for(size_t i = m; i < image.size() - m; i++){
      // for each row
      for(size_t j = m; j < image[i].size() - m; j++){
        // for each pixel in that row
        float sum = 0;
        for(int k = -m; k <= m; k++){
          for(int l = -m; l <= m; l++){
            sum += image[i+k][j+k] * kernel.get(k, l);
          }
        }
        output_image[i][j] = uint8_t(sum);
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

int main(int argc, char** argv){
  // two possible usages
  string usage1 = string(argv[0]) + " n m k";
  string usage2 = string(argv[0]) + " input_image_data.csv k";

  if(argc < 3){
    cout << "usages:" << endl;
    cout << usage1 << endl;
    cout << usage2 << endl;
  } else if(argc < 4){
    auto image = load_image(argv[1]);
    save_image(image, "output.csv");

  } else {
    // generate dummy image of size n, m and kernel of size k. Convolve them
    // and report time.
  }

  return 0;
}