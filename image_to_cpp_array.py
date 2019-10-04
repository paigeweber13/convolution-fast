import numpy as np

from skimage import filters, io

def image_to_csv(image_array: np.ndarray, output_filename: str):
    with open(output_filename, "w") as f:
        for row in image_array:
            output_string = ''
            for pixel in row:
                output_string += str(pixel) + ','

            output_string = output_string[:-2]
            output_string += '\n'
            f.write(output_string)
                
def main():
    filename = 'saturn-v-2048x2048-bw'
    image = io.imread(filename + '.jpg')
    image_to_csv(image, filename + '.csv')
    edges = filters.sobel_v(image)
    io.imsave(filename + '-expected-sobel-v.jpg', edges)

if __name__ == "__main__":
    main()