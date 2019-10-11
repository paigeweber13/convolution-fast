import numpy as np
import sys

expected_usage = sys.argv[0] + 'file-to-process.csv'
if len(sys.argv) < 2:
    print(expected_usage)
    sys.exit(1)

data = np.loadtxt(sys.argv[1], delimiter=',', skiprows=1)
mean_gigapixels_per_second = np.mean(data, axis=0)[4]
median_gigapixels_per_second = np.median(data, axis=0)[4]
max_gigapixels_per_second = np.max(data, axis=0)[4]
min_gigapixels_per_second = np.min(data, axis=0)[4]

precision = 3
print('mean_gigapixels_per_second:  ', 
      round(mean_gigapixels_per_second, precision))
print('median_gigapixels_per_second:', 
      round(median_gigapixels_per_second, precision))
print('max_gigapixels_per_second:   ',
      round(max_gigapixels_per_second, precision))
print('min_gigapixels_per_second:   ',
      round(min_gigapixels_per_second, precision))
