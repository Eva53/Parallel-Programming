#!/bin/bash
#SBATCH -J c_mpi
#SBATCH -N 4  # ���������� �����
#SBATCH -p tornado  # ��� �����
#SBATCH -o ./out
#SBATCH -e ./err
#SBATCH -t 00:01:00  # ����������� �� ����� ����������

mpicc c_mpi.c -lm -o c_mpi
mpirun -n 12 ./c_mpi input.txt c_mpi_res.txt