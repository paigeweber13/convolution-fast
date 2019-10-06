CXX=g++
CXXFLAGS=-Wall -std=c++1y -march=native -mtune=native -fopenmp -O3
CXXASSEMBLYFLAGS=-S -g -fverbose-asm
FILES=test_generate_kernels.cpp

assembly: $(FILES)
		$(CXX) $(CXXFLAGS) $(CXXASSEMBLYFLAGS) $(FILES)

compile: $(FILES)
		$(CXX) $(CXXFLAGS) $(FILES)

test: compile
		./a.out

clean:
		rm -f *.e* *.o* *.s

plot:
		python plot_data.py data/randomly-allocated-list-latency.csv data/sequentially-allocated-list-latency.csv
