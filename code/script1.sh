#!/bin/bash
#SBATCH -J c_mpi
#SBATCH -N 4  # Количество узлов
#SBATCH -p tornado  # тип узлов
#SBATCH -o ./out
#SBATCH -e ./err
#SBATCH -t 00:01:00  # Ограничение на время выполнения

mpicc c_mpi.c -lm -o c_mpi
mpirun -n 12 ./c_mpi input.txt c_mpi_res.txt