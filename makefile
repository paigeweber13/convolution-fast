include build/conanbuildinfo.mak

CXX=g++
CXXFLAGS=-g -Wall -std=c++1y -march=native -mtune=native -fopenmp -O3 -laf
CXXASSEMBLYFLAGS=-S -fverbose-asm
INCLUDES=-I/opt/arrayfire/include
LIBRARIES=-L/opt/arrayfire/lib64
FILES=$(wildcard convolution/*.cpp)
OBJS=$(FILES:.cpp=.o)
EXEC=convolution

# conan
CFLAGS          += $(CONAN_CFLAGS)
CXXFLAGS        += $(CONAN_CXXFLAGS)
INCLUDES        += $(addprefix -I, $(CONAN_INCLUDE_DIRS))
DEFINES         += $(addprefix -D, $(CONAN_DEFINES))

all: compile

assembly: $(OBJS)
		$(CXX) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CXXFLAGS)\
		$(CXXASSEMBLYFLAGS) $(FILES)

compile: $(OBJS)
		$(CXX) $(INCLUDES) $(DEFINES) $(LIBRARIES) $(CXXFLAGS) $(FILES)

%.o: %.cpp
	$(CXX) -c $< -o $@

test: compile
		$(EXEC) tests/saturn-v-2048x2048-bw.pgm 3

speedtest: compile
		$(EXEC)

clean:
		rm -f *.e* *.o* *.s
