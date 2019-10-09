CXX=g++
CXXFLAGS=-Wall -std=c++1y -march=native -mtune=native -fopenmp -O3
CXXASSEMBLYFLAGS=-S -g -fverbose-asm
FILES=$(wildcard *.cpp)

assembly: $(FILES)
		$(CXX) $(CXXFLAGS) $(CXXASSEMBLYFLAGS) $(FILES)

compile: $(FILES)
		$(CXX) $(CXXFLAGS) $(FILES)

test: compile
		./a.out ../tests/saturn-v-2048x2048-bw.csv 3

clean:
		rm -f *.e* *.o* *.s
