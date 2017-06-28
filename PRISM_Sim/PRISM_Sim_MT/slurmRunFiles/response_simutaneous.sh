#!/bin/bash

#SBATCH -J geantMTtest

#SBATCH -A ac_amsd

#SBATCH -p lr3

#SBATCH --qos=lr_normal

#SBATCH -N 48

#SBATCH -t 24:00:00

#SBATCH --exclusive


## Run the simulations!
for i in `seq 1 48`;
do
srun -n 1 -N 1 -c 16 -e slurmOut/std$i.err -o slurmOut/std$i.out ./PRISM_response -m mac/response_macros/run$i.mac -t 16 -f output/output$i.bin &
done
wait


## Stitch together the output files and get system reponse
python pythonTools/stitcher.py 16 10 output/output*.bin


## Delete the output files
rm output/output*.bin