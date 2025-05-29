#include <stdio.h>
#include <stdlib.h>

#define DEBUG 1            // comentar esta linha quando for medir tempo
#define ARRAY_SIZE 40      // trabalho final com o valores 10.000, 100.000, 1.000.000

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

	/* init array with worst case for sorting */
    for (i=0 ; i<ARRAY_SIZE; i++) vetor[i] = ARRAY_SIZE-i;

    #ifdef DEBUG
	/* print unsorted array */
    printf("\nVetor original:\n");
    for (i=0 ; i<ARRAY_SIZE; i++) printf("[%03d] ", vetor[i]);
    #endif
 
	printf("\n");

	/* sort array */
    bs(ARRAY_SIZE, vetor);

    #ifdef DEBUG
	/* print sorted array */
    printf("\nVetor sorted:\n");
    for (i=0 ; i<ARRAY_SIZE; i++) printf("[%03d] ", vetor[i]);
    #endif

	printf("\n");
    return 0;
}
