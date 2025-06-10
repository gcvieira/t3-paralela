# Trabalho 3

Trabalho sobre ordenacao de vetores utilizando computacao paralela utilizando MPI e analise de balanceamento de carga.

## Running

Sequencial:
	gcc sequencial.c -o s
	./s

mpi:
	ladcomp -env mpicc mpiSimples.c -o mpis
	ladcomp -env mpicc mpi_optimized.c -o mpio -lm
	srun -N 1 -n 7 ./mpis
	srun -N 1 -n 8 ./mpio 90000
