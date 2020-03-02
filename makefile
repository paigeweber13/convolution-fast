#include build/conanbuildinfo.mak

CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -march=native -mtune=native -fopenmp -O3 $(LIBRARY_FLAGS) -DLIKWID_PERFMON
CUDAFLAGS=-g -std=c++11 -Xcompiler "-Wall -march=native -mtune=native -fopenmp -O3"
#CXXASSEMBLYFLAGS=-S -fverbose-asm
CUDAC=nvcc
# INCLUDES=-I/opt/arrayfire/include
# LIBRARIES=-L/opt/arrayfire/lib64
# LIBRARY_FLAGS=-laf
INCLUDES=-I/home/riley/code/fine-hardware-visualization/lib
LIBRARIES=-L/usr/local/lib
LIBRARY_FLAGS=-llikwid
FILES=$(wildcard src/*.cpp) $(wildcard /home/riley/code/fine-hardware-visualization/lib/*.cpp) 
CUDA_FILES=$(wildcard src/*.cu)
OBJS=$(FILES:.cpp=.o)
CUDA_OBJS=$(FILES:.cu=.o)
EXEC=convolution.out

# conan
# CFLAGS          += $(CONAN_CFLAGS)
# CXXFLAGS        += $(CONAN_CXXFLAGS)
# INCLUDES        += $(addprefix -I, $(CONAN_INCLUDE_DIRS))
# DEFINES         += $(addprefix -D, $(CONAN_DEFINES))

#assembly: $(OBJS)
#	$(CXX) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CXXFLAGS)\
#	$(CXXASSEMBLYFLAGS) $(FILES)

cpu: $(OBJS)
	$(CXX) $(INCLUDES) $(FILES) $(LIBRARIES) $(DEFINES) $(CXXFLAGS) -o $(EXEC)

gpu: $(OBJS) $(CUDA_OBJS)
	$(CUDAC) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CUDAFLAGS) $(FILES) \
	$(CUDA_FILES) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CXXFLAGS) -c $< -o $@

%.o: %.cu
	$(CUDAC) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CUDAFLAGS) -c $< -o $@

test: cpu
	# ./queue.sh convolution.sh
	./convolution.out -i tests/saturn-v-2048x2048-bw.pgm -o tests/output.pgm


clean:
	rm -f *.e* *.o* *.s
