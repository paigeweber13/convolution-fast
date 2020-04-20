#!/bin/bash
CSV_HEADER="         m,          n,  k,runtime (s),  MP/second"

# Image size of 1,000,000,000 (one billion) pixels would take about 4 Gib of
# memory. An image of size 31,600 on each dimension would be about 1 billion
# pixels

# 2^15 is 32,768. That should still fit in memory since I have 8 Gib

FILENAME=data/speedtest_$( date +"%Y-%m-%d_%H%M" ).csv
IFS=
echo $CSV_HEADER > $FILENAME

IMAGE_SIZE=512
while [[ $IMAGE_SIZE -lt 33000 ]]; do
  for KERNEL_SIZE in {3..15..2}; do
    ./convolution.out -s $IMAGE_SIZE $IMAGE_SIZE $KERNEL_SIZE >> $FILENAME
  done
  let "IMAGE_SIZE = $IMAGE_SIZE * 2"
done
