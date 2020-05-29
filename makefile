#include build/conanbuildinfo.mak

CXX=g++
CXXFLAGS=-g -DLIKWID_CLI -std=c++14 -Wall -march=native -mtune=native -fopenmp -O3 $(LIBRARY_FLAGS) -DLIKWID_PERFMON
CUDAFLAGS=-g -std=c++14 -Xcompiler "-Wall -march=native -mtune=native -fopenmp -O3"
#CXXASSEMBLYFLAGS=-S -fverbose-asm
CUDAC=nvcc
# INCLUDES=-I/opt/arrayfire/include
# LIBRARIES=-L/opt/arrayfire/lib64
# LIBRARY_FLAGS=-laf
INCLUDES=-I/home/riley/code/fine-hardware-visualization/lib
INCLUDES+=-I../lib
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
	$(CXX) $(INCLUDES) $(OBJS) $(LIBRARIES) $(DEFINES) $(CXXFLAGS) -o $(EXEC)

gpu: $(OBJS) $(CUDA_OBJS)
	$(CUDAC) $(INCLUDES) $(CUDA_OBJS) $(OBJS) $(DEFINES) $(LIBRARIES) \
	$(CUDAFLAGS) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CXXFLAGS) -c $< -o $@

%.o: %.cu
	$(CUDAC) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CUDAFLAGS) -c $< -o $@

test: cpu
	# ./queue.sh convolution.sh
	./convolution.out -i tests/saturn-v-2048x2048-bw.pgm -o tests/output.pgm

likwid-test: cpu
	likwid-perfctr -C S0:0-3 -g PORT_USAGE1 -g PORT_USAGE2 -g PORT_USAGE3 -M 1 -m "./convolution.out -i tests/saturn-v-2048x2048-bw.pgm -o tests/output.pgm"

likwid-speedtest: cpu
	likwid-perfctr -C S0:0-3 -g PORT_USAGE1 -g PORT_USAGE2 -g PORT_USAGE3 -M 1 -m "./convolution.out -s 2000 2000 9"

clean:
	rm -f *.e* *.o* *.s $(OBJS) $(EXEC)
