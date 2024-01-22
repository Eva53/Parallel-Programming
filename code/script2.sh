#!/bin/bash
#SBATCH -J c_mpi
#SBATCH -N 4  # Количество узлов
#SBATCH -p tornado  # тип узлов
#SBATCH -o ./out
#SBATCH -e ./err
#SBATCH -t 00:01:00  # Ограничение на время выполнения

mpirun -n 12 python3 py_mpi.py