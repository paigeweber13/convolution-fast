#include build/conanbuildinfo.mak

CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -march=native -mtune=native -fopenmp -O3 -laf
CUDAFLAGS=-g -std=c++11 -Xcompiler "-Wall -march=native -mtune=native -fopenmp -O3"
#CXXASSEMBLYFLAGS=-S -fverbose-asm
CUDAC=nvcc
INCLUDES=-I/opt/arrayfire/include
LIBRARIES=-L/opt/arrayfire/lib64
FILES=$(wildcard src/*.cpp) $(wildcard src/*.cu)
OBJS=$(FILES:.cpp=.o) $(FILES:.cu=.o)
EXEC=convolution.out

# conan
CFLAGS          += $(CONAN_CFLAGS)
CXXFLAGS        += $(CONAN_CXXFLAGS)
INCLUDES        += $(addprefix -I, $(CONAN_INCLUDE_DIRS))
DEFINES         += $(addprefix -D, $(CONAN_DEFINES))

all: $(EXEC)

#assembly: $(OBJS)
#		$(CXX) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CXXFLAGS)\
#		$(CXXASSEMBLYFLAGS) $(FILES)

$(EXEC): $(OBJS)
		$(CUDAC) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CUDAFLAGS) $(FILES) -o $@

%.o: %.cpp
	$(CUDAC) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CUDAFLAGS) -c $< -o $@

%.o: %.cu
	$(CUDAC) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CUDAFLAGS) -c $< -o $@

test: $(EXEC)
		$(EXEC) tests/saturn-v-2048x2048-bw.pgm 3

speedtest: $(EXEC)
		$(EXEC)

clean:
		rm -f *.e* *.o* *.s
