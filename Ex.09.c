#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define TAMANHO 100000000
#define MAX 100

int main() {
    time_t t;
    srand((unsigned) time(&t));
    long int *vetor;
    vetor = (long int*)malloc(TAMANHO * sizeof(long int));
    int i;
    int x = 1 + (rand() % MAX);
    double countSerial = 0.0, countParalela = 0.0;
    for(i = 0; i < TAMANHO; i++) {
        int num = 1 + (rand() % MAX);
        vetor[i] = num;
    }
    // Execução Serial
    double inicio = omp_get_wtime();
    for(i = 0; i <= TAMANHO; i++) {
        if(vetor[i] == x) {
            countSerial++;
        }
    }
    double fim = omp_get_wtime();
    double tSerial = fim - inicio;
    printf("Execucao Sequencial: %fseg\n", tSerial);
    printf("x foi encontrado %.0f vez(es)\n", countSerial);
    
    //Execução Paralela
    inicio = omp_get_wtime();
    #pragma omp parallel num_threads(3)
    {
        double soma = 0;
        #pragma omp for
            for(i = 0; i <= TAMANHO; i++) {
                if(vetor[i] == x) {
                    soma += 1;
                }
            }
        #pragma omp critical
            countParalela += soma;
    }
    fim = omp_get_wtime();
    double tParalelo = fim - inicio;
    double speedup = tSerial/tParalelo;
    printf("x foi encontrado %.0f vez(es)\n", countParalela);
    printf("Execucao paralela: %fseg\n",tParalelo);
    printf("Speedup: %f\n", speedup);
    printf("Eficiencia: %f\n",speedup/3.0);
    printf("\n");

    return 0;
}