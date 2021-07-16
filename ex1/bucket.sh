#!/bin/bash
#SBATCH -J bucket # nombre del job
#SBATCH -p investigacion # nombre de la particion 
#SBATCH -N 2 # numero de nodos
#SBATCH --tasks-per-node=8 # numero de tasks por nodo

	
module load gcc/8.4.0
module load openmpi/3.1.5
module load python/3.7.7

python measure_bucket.py

module unload gcc/8.4.0
module unload openmpi/3.1.5
module unload python/3.7.7
