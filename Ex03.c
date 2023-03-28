#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define TAMANHO 500000000
#define MAX 100000

int main () {
    time_t t;
    srand((unsigned) time(&t));
    long int *vetor;
    vetor = (long int*)malloc(TAMANHO * sizeof(long int));
    int i;
    double incio = 0.0, fim = 0.0;
    double somaSerial = 0.0, somaParalela = 0.0;
    double mediaSerial = 0.0, mediaParalela = 0.0;
    for(i = 0; i < TAMANHO; i++) {
        int num = 1 + (rand() % (MAX + 1));
        vetor[i] = num;
    }
    
    // Execução Serial
    incio = omp_get_wtime();
    for(i = 0; i < TAMANHO; i++) {
        somaSerial += vetor[i];
    }
    mediaSerial = somaSerial / TAMANHO;
    fim = omp_get_wtime();
    double tSerial = fim - incio;
    printf("Execucao Sequencial: %fseg\n", tSerial);
    printf("Media: %f\n", mediaSerial);

    //Execucao Paralela
    incio = omp_get_wtime();
    #pragma omp parallel num_threads(4)
    {
        #pragma omp for reduction (+:somaParalela)
            for (i = 0; i < TAMANHO; i++) {
                somaParalela += vetor[i];
            }
        
    }
    mediaParalela = somaParalela / TAMANHO;
    fim = omp_get_wtime();
    double tParalelo = fim - incio;
    double speedup = tSerial / tParalelo;
    printf("Execucao Paralela: %fseg\n", tParalelo);
    printf("Media: %f\n", mediaParalela);
    printf("Speedup: %f\n", speedup);
    printf("Eficiencia: %f\n", speedup / 4);

}