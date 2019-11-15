#!/bin/bash
./convolution.out -i tests/saturn-v-2048x2048-bw.pgm -o tests/saturn-v-out.pgm
./convolution.out -s 1024 1024 7
 
# ./convolution.out -a g -i tests/saturn-v-2048x2048-bw.pgm -o tests/saturn-v-out.pgm
# ./convolution.out -a g -s 1024 1024 7

