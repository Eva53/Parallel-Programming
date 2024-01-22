#!/bin/bash
#SBATCH -J c_mpi
#SBATCH -N 4  # ���������� �����
#SBATCH -p tornado  # ��� �����
#SBATCH -o ./out
#SBATCH -e ./err
#SBATCH -t 00:01:00  # ����������� �� ����� ����������

mpirun -n 12 python3 py_mpi.py