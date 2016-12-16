#!/bin/bash

#SBATCH -J geantMTtest

#SBATCH -A ac_amsd

#SBATCH -p lr2

#SBATCH --qos=lr_normal

#SBATCH -N 1

#SBATCH -e std.err

#SBATCH -o std.out

#SBATCH -t 24:00:00

./PRISM_response -m run1.mac -t 12 -f output1.txt
./PRISM_response -m run2.mac -t 12 -f output2.txt