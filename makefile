#include build/conanbuildinfo.mak

CXX=nvcc
CXXFLAGS=-g -std=c++14 -Xcompiler "-Wall -march=native -mtune=native -fopenmp -O3 -laf -lboost_options"
#CXXASSEMBLYFLAGS=-S -fverbose-asm
CUDAC=nvcc
INCLUDES=-I/opt/arrayfire/include
LIBRARIES=-L/opt/arrayfire/lib64
FILES=$(wildcard src/*.cpp) $(wildcard src/*.cu)
OBJS=$(FILES:.cpp=.o) $(FILES:.cu=.o)
EXEC=convolution

# conan
CFLAGS          += $(CONAN_CFLAGS)
CXXFLAGS        += $(CONAN_CXXFLAGS)
INCLUDES        += $(addprefix -I, $(CONAN_INCLUDE_DIRS))
DEFINES         += $(addprefix -D, $(CONAN_DEFINES))

all: compile

#assembly: $(OBJS)
#		$(CXX) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CXXFLAGS)\
#		$(CXXASSEMBLYFLAGS) $(FILES)

compile: $(OBJS)
		$(CXX) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CUDAFLAGS) $(FILES)
		# $(CXX) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CXXFLAGS) $(FILES)

%.o: %.cpp
	$(CUDAC) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CUDAFLAGS) -c $< -o $@

%.o: %.cu
	$(CUDAC) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CUDAFLAGS) -c $< -o $@

test: compile
		$(EXEC) tests/saturn-v-2048x2048-bw.pgm 3

speedtest: compile
		$(EXEC)

clean:
		rm -f *.e* *.o* *.s
