#include <stdio.h>
#include <stdlib.h>

#define DEBUG 1            // comentar esta linha quando for medir tempo
#define ARRAY_SIZE 40      // trabalho final com o valores 10.000, 100.000, 1.000.000

/* recebe um ponteiro para um vetor que contem as mensagens recebidas dos filhos e            */
/* intercala estes valores em um terceiro vetor auxiliar. Devolve um ponteiro para este vetor */

int *interleaving(int vetor[], int tam) {
	int *vetor_auxiliar;
	int i1, i2, i_aux;

	vetor_auxiliar = (int *)malloc(sizeof(int) * tam);

	i1 = 0;
	i2 = tam / 2;
	for (i_aux = 0; i_aux < tam; i_aux++) {
		if (((vetor[i1] <= vetor[i2]) && (i1 < (tam / 2)))
		    || (i2 == tam))
			vetor_auxiliar[i_aux] = vetor[i1++];
		else
			vetor_auxiliar[i_aux] = vetor[i2++];
	}
	return vetor_auxiliar;
}

void bs(int n, int * vetor) {
    int c=0, d, troca, trocou =1;
    while (c < (n-1) & trocou ) {
        trocou = 0;
        for (d = 0 ; d < n - c - 1; d++)
            if (vetor[d] > vetor[d+1])
                {
                troca      = vetor[d];
                vetor[d]   = vetor[d+1];
                vetor[d+1] = troca;
                trocou = 1;
                }
        c++;
        }
}

int main() {
    int vetor[ARRAY_SIZE];
    int i;
	/*
    for (i=0 ; i<ARRAY_SIZE; i++) vetor[i] = ARRAY_SIZE-i;
    #ifdef DEBUG
	// print unsorted array
    printf("\nVetor original:\n");
    for (i=0 ; i<ARRAY_SIZE; i++) printf("[%03d] ", vetor[i]);
	printf("\n");
    #endif
	// sort array
    bs(ARRAY_SIZE, vetor);
    #ifdef DEBUG
	// print sorted array
    printf("\nVetor sorted:\n");
    for (i=0 ; i<ARRAY_SIZE; i++) printf("[%03d] ", vetor[i]);
	printf("\n");
    #endif
	*/

	MPI_Init();
	my_rank = MPI_Comm_rank();  // pega pega o numero do processo atual (rank)

	// recebo vetor
	if ( my_rank != 0 ) {
		MPI_Recv(vetor, pai);                        // não sou a raiz, tenho pai
		MPI_Get_count(&Status, MPI_INT, &tam_vetor);  // descubro tamanho da mensagem recebida
	} else {
		tam_vetor = VETOR_SIZE;        // defino tamanho inicial do vetor
		//inicializaVetor(vetor, tam_vetor);  // sou a raiz e portanto gero o vetor - ordem reversa
		for (i=0 ; i<ARRAY_SIZE; i++) vetor[i] = ARRAY_SIZE-i;

		printf("\nVetor original:\n");
		for (i=0 ; i<ARRAY_SIZE; i++) printf("[%03d] ", vetor[i]);
		printf("\n");
	}

	// dividir ou conquistar?
	if ( tam_vetor <= delta ) BubbleSort(vetor);  // conquisto
	else {
		// dividir
		// quebrar em duas partes e mandar para os filhos
		MPI_Send(&vetor[0], filho esquerda, tam_vetor/2);  // mando metade inicial do vetor
		MPI_Send(&vetor[tam_vetor/2], filho direita, tam_vetor/2);  // mando metade final

		// receber dos filhos
		MPI_Recv(&vetor[0], filho esquerda);
		MPI_Recv(&vetor[tam_vetor/2], filho direita);

		// intercalo vetor inteiro
		//intercala(vetor);
		int *vetor_auxiliar; // ponteiro para o vetor resultantes que sera alocado dentro da rotina
		vetor_auxiliar = interleaving(vetor, tam_vetor);
	}

	// mando para o pai
	if ( my_rank !=0 ) MPI_Send(vetor,pai,tam_vetor);  // tenho pai, retorno vetor ordenado pra ele
	else {
		//Mostra(vetor);  // sou o raiz, mostro vetor
		// TODO: review: esse maybe eh o vetor_auxiliar
		printf("\nVetor sorted:\n");
		for (i=0 ; i<ARRAY_SIZE; i++) printf("[%03d] ", vetor[i]);
		printf("\n");
	}
	MPI_Finalize();
	return 0;
}
