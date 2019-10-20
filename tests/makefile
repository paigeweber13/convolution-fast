CXX=g++
CXXFLAGS=-Wall -g -std=c++1y -march=native -mtune=native -fopenmp -O3
CXXASSEMBLYFLAGS=-S -fverbose-asm
FILES=tests.cpp ../convolution/kernel.cpp ../convolution/generate_kernels.cpp

assembly: $(FILES)
		$(CXX) $(CXXFLAGS) $(CXXASSEMBLYFLAGS) $(FILES)

compile: $(FILES)
		$(CXX) $(CXXFLAGS) $(FILES)

test: compile
		./a.out

clean:
		rm -f *.e* *.o* *.s
