CXX=g++
CXXFLAGS=-g -Wall -std=c++1y -march=native -mtune=native -fopenmp -O3
CXXASSEMBLYFLAGS=-S -fverbose-asm
INCLUDES=-I $$HOME/software/boost_1_71_0
FILES=$(wildcard convolution/*.cpp)

assembly: $(FILES)
		$(CXX) $(INCLUDES) $(CXXFLAGS) $(CXXASSEMBLYFLAGS) $(FILES)

compile: $(FILES)
		$(CXX) $(INCLUDES) $(CXXFLAGS) $(FILES)

test: compile
		./a.out tests/saturn-v-2048x2048-bw.pgm 3

speedtest: compile
		./a.out

clean:
		rm -f *.e* *.o* *.s
