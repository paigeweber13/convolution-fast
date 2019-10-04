import matplotlib.pyplot as plt

def flop_perf_bound(m, n, k):
    return n * m/(n * m * 2 * k * k /1.64e12)

def mem_perf_bound(m, n, k, datatype_size):
    return n * m/((n * m * datatype_size/3.3e10) + (k*k*datatype_size/1e11))

def plot_as_function_of_k(m, n, datatype_size):
    x = []
    y_flop = []
    y_mem = []
    for k in range(1,21):
        x.append(k)
        y_flop.append(flop_perf_bound(m,n,k))
        y_mem.append(mem_perf_bound(m,n,k,datatype_size))

    plt.figure(figsize=(15,7))
    plt.yscale('log')
    plt.title('Performance Bounds as Functions of Convolution Dimension')
    plt.ylabel('Pixels/s')
    plt.xlabel('k')
    plt.plot(x, y_flop)
    plt.plot(x, y_mem)
    plt.legend(['Flop Performance Bound', 'Memory Performance Bound'])
    plt.xticks(range(1, 21))
    plt.show()

if __name__ == '__main__':
    plot_as_function_of_k(1024, 768, 1)