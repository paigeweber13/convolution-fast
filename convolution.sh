#!/bin/bash
echo "hostname: $(hostname)"
echo ""

echo "------ CPU: ------"
echo "running tests"
./convolution.out -t tests/saturn-v-2048x2048-bw.pgm
echo ""
echo "convolving actual image"
./convolution.out -i tests/saturn-v-2048x2048-bw.pgm -o tests/saturn-v-out.pgm
echo ""
echo "running speedtests"
echo "        m,        n,   k,   time (s), performance (Mp/s)"
./convolution.out -s 1024 1024 7
echo ""
 
echo "------ GPU: ------"
echo "running tests"
./convolution.out -a g -t tests/saturn-v-2048x2048-bw.pgm
echo ""
echo "convolving actual image"
# ./convolution.out -a g -i tests/saturn-v-2048x2048-bw.pgm -o tests/saturn-v-out.pgm
echo ""
echo "running speedtests"
echo "        m,        n,   k,   time (s), performance (Mp/s)"
# ./convolution.out -a g -s 1024 1024 7
echo ""

