#!/bin/bash
#SBATCH --job-name=example
#SBATCH --partition=sandbox
## 3 day max run time for public partitions, except 4 hour max runtime for the sandbox partition
#SBATCH --time=0-01:00:00 ## time format is DD-HH:MM:SS
#SBATCH --cpus-per-task=1
#SBATCH --mem=6400 ## max amount of memory per node you require

for i in {1..10}
do
	./$1 $2 $3
done