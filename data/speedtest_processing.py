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
# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')
# ax.scatter(n_by_m, k, gigapixels_per_second)
# ax.set_xlabel('n*m (approx. bytes loaded)')
# ax.set_ylabel('k')
# ax.set_zlabel('gigapixels/s')
# plt.show()

### some projections
# common variables:
colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k']

# gigapixels per second per n*m for each grouping of k
data_by_k = {}

possible_k = sorted(np.unique(data[:,2]))

legend_values = []
for k in possible_k:
    legend_values.append('k: ' + str(k))
    data_by_k[k] = np.array([x for x in data if x[2] == k])

plt.figure(figsize=(15,9))
i = 0
for key in data_by_k:
    plt.scatter(data_by_k[key][:,0] * data_by_k[key][:,0],
                data_by_k[key][:,4], c=colors[i], marker='.')
    i += 1

plt.legend(legend_values)
plt.xscale('log')
plt.xlabel('n*m (approx. bytes loaded)')
plt.ylabel('performance in gigapixels per second')
plt.show()


# gigapixels per second per k for each grouping of n*m
data_by_size = {}

image_sizes = {
    (1024, 768),
    (2048, 2048),
    (8192, 8192),
    (4194304 , 768),
    (16777216 , 768),
}

legend_values = []
for m, n in image_sizes:
    legend_values.append(str(m) + ' x ' + str(n) + ' pixels')
    new_entry = np.array([x for x in data if x[0] == m and x[1] == n])
    data_by_size[(m, n)] = new_entry

data_by_size_keys_sorted = sorted(data_by_size.keys())

plt.figure(figsize=(15,9))
i = 0
for key in data_by_size_keys_sorted:
    plt.plot(data_by_size[key][:,2], data_by_size[key][:,4], c=colors[i], 
             marker='.')
    i += 1

plt.legend(legend_values)
plt.xlabel('k')
plt.ylabel('performance in gigapixels per second')
plt.show()
