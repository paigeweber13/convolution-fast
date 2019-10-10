#!/bin/bash
qsub -q mamba -d $(pwd) -l nodes=1:ppn=16 -l walltime=00:20:00 $1
