#include <stdio.h>

int conta_processos(int tam, int delta) {
	if (tam <= delta) return 1;
	else return 1 + 2 * conta_processos(tam / 2, delta);
}

int main() {
	int tam = 800;
	int delta = 100;
	int total = conta_processos(tam, delta);
	printf("Para vetor de tamanho %d e delta = %d, número exato de processos = %d\n", tam, delta, total);
	return 0;
}
