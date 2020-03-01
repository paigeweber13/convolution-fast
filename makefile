#include build/conanbuildinfo.mak

CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -march=native -mtune=native -fopenmp -O3 
# below line is with arrayfire
# CXXFLAGS=-g -std=c++11 -Wall -march=native -mtune=native -fopenmp -O3 -laf
CUDAFLAGS=-g -std=c++11 -Xcompiler "-Wall -march=native -mtune=native -fopenmp -O3"
#CXXASSEMBLYFLAGS=-S -fverbose-asm
CUDAC=nvcc
# INCLUDES=-I/opt/arrayfire/include
# LIBRARIES=-L/opt/arrayfire/lib64
FILES=$(wildcard src/*.cpp) 
CUDA_FILES=$(wildcard src/*.cu)
OBJS=$(FILES:.cpp=.o)
CUDA_OBJS=$(FILES:.cu=.o)
EXEC=convolution.out

# conan
CFLAGS          += $(CONAN_CFLAGS)
CXXFLAGS        += $(CONAN_CXXFLAGS)
INCLUDES        += $(addprefix -I, $(CONAN_INCLUDE_DIRS))
DEFINES         += $(addprefix -D, $(CONAN_DEFINES))

#assembly: $(OBJS)
#	$(CXX) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CXXFLAGS)\
#	$(CXXASSEMBLYFLAGS) $(FILES)

cpu: $(OBJS)
	$(CXX) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CXXFLAGS) $(FILES) -o $(EXEC)

gpu: $(OBJS) $(CUDA_OBJS)
	$(CUDAC) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CUDAFLAGS) $(FILES) \
	$(CUDA_FILES) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CXXFLAGS) -c $< -o $@

%.o: %.cu
	$(CUDAC) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CUDAFLAGS) -c $< -o $@

test: $(EXEC)
	./queue.sh convolution.sh

clean:
	rm -f *.e* *.o* *.s
