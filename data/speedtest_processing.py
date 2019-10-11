from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
import sys

expected_usage = sys.argv[0] + 'file-to-process.csv'
if len(sys.argv) < 2:
    print(expected_usage)
    sys.exit(1)

data = np.loadtxt(sys.argv[1], delimiter=',', skiprows=1)

# give things useful names
n_by_m = data[:,0] * data[:,1]
k = data[:,2]
gigapixels_per_second = data[:,4]

# calculate stuff
mean_gigapixels_per_second = np.mean(gigapixels_per_second)
median_gigapixels_per_second = np.median(gigapixels_per_second)
max_gigapixels_per_second = np.max(gigapixels_per_second)
min_gigapixels_per_second = np.min(gigapixels_per_second)

precision = 3
print('mean_gigapixels_per_second:  ', 
      round(mean_gigapixels_per_second, precision))
print('median_gigapixels_per_second:', 
      round(median_gigapixels_per_second, precision))
print('max_gigapixels_per_second:   ',
      round(max_gigapixels_per_second, precision))
print('min_gigapixels_per_second:   ',
      round(min_gigapixels_per_second, precision))

# 3d plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(n_by_m, k, gigapixels_per_second)
ax.set_xlabel('n*m (approx. bytes loaded)')
ax.set_ylabel('k')
ax.set_zlabel('gigapixels/s')
plt.show()

# some projections
# gigapixels per second per n*m
plt.scatter(n_by_m, gigapixels_per_second)
# trend_line_coefficients = \
#     np.polynomial.polynomial.polyfit(n_by_m, gigapixels_per_second, 2)
# trend_line_function = np.poly1d(trend_line_coefficients)
# plt.plot(np.unique(n_by_m), trend_line_function(np.unique(n_by_m)))
plt.xlabel('n*m (approx. bytes loaded)')
plt.ylabel('performance in gigapixels per second')
plt.show()

# gigapixels per second per k
plt.scatter(k, gigapixels_per_second)
# trend_line_coefficients = \
#     np.polynomial.polynomial.polyfit(k, gigapixels_per_second, 2)
# trend_line_function = np.poly1d(trend_line_coefficients)
# plt.plot(np.unique(k), trend_line_function(np.unique(k)))
plt.xlabel('k')
plt.ylabel('performance in gigapixels per second')
plt.show()
