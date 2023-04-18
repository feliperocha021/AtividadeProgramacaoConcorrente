#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define TAMANHO 100000000
#define MAX 10000

int main() {
    time_t t;
    srand((unsigned) time(&t));
    long int *vetor;
    vetor = (long int*)malloc(TAMANHO * sizeof(long int));
    int i;
    int maiorSerial, maiorParalelo;
    long int indiceSerial, indiceParalelo;
    for(i = 0; i < TAMANHO; i++) {
        int num = 1 + (rand() % MAX);
        vetor[i] = num;
    }

    // Execução Serial
    maiorSerial = vetor[0];
    double inicio = omp_get_wtime();
    for(i = 1; i < TAMANHO; i++) {
        if(vetor[i] > maiorSerial) {
            maiorSerial = vetor[i];
            indiceSerial = i;
        }
    }
    double fim = omp_get_wtime();
    double tSerial = fim - inicio;
    printf("Execucao Sequencial: %fseg\n", tSerial);
    printf("Maior valor encontrado: %d no indice %d\n", maiorSerial, indiceSerial);
    
    //Execução Paralela
    maiorParalelo = vetor[0];
    inicio = omp_get_wtime();
    #pragma omp parallel num_threads(3)
    {   
        double maior = vetor[0];
        long int indice = TAMANHO;
        #pragma omp for
            for(i = 1; i < TAMANHO; i++) {
                if(vetor[i] > maior) {
                    maior = vetor[i];
                    indice = i; 
                }
            }
        #pragma omp critical
            if(maior > maiorParalelo) {
                maiorParalelo = maior;
                indiceParalelo = indice;
            }
    }
    fim = omp_get_wtime();
    double tParalelo = fim - inicio;
    double speedup = tSerial/tParalelo;
    printf("Execucao paralela: %fseg\n",tParalelo);
    printf("Maior valor encontrado: %d no indice %d\n", maiorParalelo, indiceParalelo);
    printf("Speedup: %f\n", speedup);
    printf("Eficiencia: %f\n",speedup/3.0);
    printf("\n");

    return 0;
}