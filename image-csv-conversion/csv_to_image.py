import csv
import numpy as np
import sys

from skimage import filters, io

def load_csv(input_filename: str):
    with open(input_filename) as f:
        reader = csv.reader(f)
        x = list(reader)
    result = np.array(x).astype('uint8')
    return result

def load_and_convert(filename):
    image_array = load_csv(filename + '.csv')
    print(image_array)
    io.imsave(filename + '.jpg', image_array)
                
def main():
    if len(sys.argv) < 2:
        print("usage:", sys.argv[0], "file-to-convert.csv")
        sys.exit(1)
    print("converting file:", sys.argv[1][:-4])
    load_and_convert(sys.argv[1][:-4])

if __name__ == "__main__":
    main()