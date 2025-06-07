#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define DEBUG 1
#define ARRAY_SIZE 40

int *interleaving(int vetor[], int tam) {
	int *vetor_auxiliar = (int *)malloc(sizeof(int) * tam);
	int i1 = 0, i2 = tam / 2, i_aux;

	for (i_aux = 0; i_aux < tam; i_aux++) {
		if (((vetor[i1] <= vetor[i2]) && (i1 < (tam / 2))) || (i2 == tam))
			vetor_auxiliar[i_aux] = vetor[i1++];
		else
			vetor_auxiliar[i_aux] = vetor[i2++];
	}
	return vetor_auxiliar;
}

void bs(int n, int *vetor) {
	int c = 0, d, troca, trocou = 1;
	while (c < (n - 1) && trocou) {
		trocou = 0;
		for (d = 0; d < n - c - 1; d++) {
			if (vetor[d] > vetor[d + 1]) {
				troca = vetor[d];
				vetor[d] = vetor[d + 1];
				vetor[d + 1] = troca;
				trocou = 1;
			}
		}
		c++;
	}
}

int main(int argc, char **argv) {
	int vetor[ARRAY_SIZE];
	int tam_vetor = 0;
	int my_rank, pai = -1, proc_n, i;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

	if (my_rank != 0) {
		// Recebe vetor do pai (de tamanho desconhecido)
		MPI_Recv(&vetor[0], ARRAY_SIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &tam_vetor);  // agora sabemos o tamanho real
		pai = status.MPI_SOURCE;  // salva quem é o pai

	} else {
		tam_vetor = ARRAY_SIZE;
		for (i = 0; i < ARRAY_SIZE; i++) vetor[i] = ARRAY_SIZE - i;

#ifdef DEBUG
		printf("\nVetor original:\n");
		for (i = 0; i < ARRAY_SIZE; i++) printf("[%03d] ", vetor[i]);
		printf("\n");
#endif
	}

	int delta = 10;

	if (tam_vetor <= delta) {
		bs(tam_vetor, vetor);  // ordena localmente
	} else {
		int filho_esquerda = my_rank * 2 + 1;
		int filho_direita = my_rank * 2 + 2;

		// envia metade do vetor para cada filho
		MPI_Send(&vetor[0], tam_vetor / 2, MPI_INT, filho_esquerda, 0, MPI_COMM_WORLD);
		MPI_Send(&vetor[tam_vetor / 2], tam_vetor / 2, MPI_INT, filho_direita, 0, MPI_COMM_WORLD);

		// recebe os vetores ordenados de volta
		MPI_Recv(&vetor[0], tam_vetor / 2, MPI_INT, filho_esquerda, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(&vetor[tam_vetor / 2], tam_vetor / 2, MPI_INT, filho_direita, 0, MPI_COMM_WORLD, &status);

		// intercalamos
		int *vetor_auxiliar = interleaving(vetor, tam_vetor);
		for (i = 0; i < tam_vetor; i++) vetor[i] = vetor_auxiliar[i];
		free(vetor_auxiliar);
	}

	// envia resultado ao pai
	if (my_rank != 0) {
		MPI_Send(vetor, tam_vetor, MPI_INT, pai, 0, MPI_COMM_WORLD);
	} else {
#ifdef DEBUG
		printf("\nVetor sorted:\n");
		for (i = 0; i < ARRAY_SIZE; i++) printf("[%03d] ", vetor[i]);
		printf("\n");
#endif
	}

	MPI_Finalize();
	return 0;
}
