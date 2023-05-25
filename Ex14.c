#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define TAMANHO 1000000
#define MAX 100000

int *criarVetor(int tam) {
    int *vetor = malloc(tam*sizeof(int));
    for (int i = 0; i < tam ; i++) {
        int num = (rand() % (MAX + 1));
        vetor[i] = num;
    }
    return (vetor);
}

int main() {
    MPI_Init(NULL, NULL);
    int nprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    
    time_t t;
    srand((unsigned) time(&t)*rank);
    int qtdVetores = (int)(TAMANHO / nprocs);
    int *subVetor = NULL;
    subVetor = criarVetor(qtdVetores);

    int soma = 0;
    for (int i = 0; i < qtdVetores; i++) {
        soma = soma + subVetor[i];
    }

    double media;
    media = soma / (double)qtdVetores;
    media = media / nprocs;

    double mediaTotal;
    MPI_Allreduce(&media, &mediaTotal, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    double somaProcesso = 0;
    for (int i = 0; i < qtdVetores; i++) {
        somaProcesso = somaProcesso + pow(subVetor[i] - mediaTotal, 2);
    }

    double somaTotal;
    MPI_Reduce(&somaProcesso, &somaTotal, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        double desvio;
        desvio = somaTotal / (double)TAMANHO;
        printf("Desvio padrao: %f\n", sqrt(desvio));
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}