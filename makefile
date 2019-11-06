include build/conanbuildinfo.mak

CXX=g++
CXXFLAGS=-g -Wall -std=c++1y -march=native -mtune=native -fopenmp -O3
CXXASSEMBLYFLAGS=-S -fverbose-asm
FILES=$(wildcard convolution/*.cpp)

# conan
CFLAGS          += $(CONAN_CFLAGS)
CXXFLAGS        += $(CONAN_CXXFLAGS)
INCLUDES        += $(addprefix -I, $(CONAN_INCLUDE_DIRS))
DEFINES         += $(addprefix -D, $(CONAN_DEFINES))

assembly: $(FILES)
		$(CXX) $(INCLUDES) $(DEFINES) $(CXXFLAGS) $(CXXASSEMBLYFLAGS) $(FILES)

compile: $(FILES)
		$(CXX) $(INCLUDES) $(DEFINES) $(CXXFLAGS) $(FILES)

test: compile
		./a.out tests/saturn-v-2048x2048-bw.pgm 3

speedtest: compile
		./a.out

clean:
		rm -f *.e* *.o* *.s
