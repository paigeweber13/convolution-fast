#!/bin/bash
qsub -q mamba -d $(pwd) -l nodes=1:ppn=16 -l walltime=01:00:00 $1
