#!/bin/bash
#SBATCH -J bitonic
#SBATCH -p investigacion
#SBATCH -N 2
#SBATCH --tasks-per-node=8

module load gcc/8.4.0
module load openmpi/3.1.5
module load python/3.7.7
module load mpich/3.3.2

unset OMP_NUM_THREADS

./bitonic ${SLURM_NPROCS}

module unload gcc/8.4.0
module unload openmpi/3.1.5
module unload python/3.7.7
module unload mpich/3.3.2







