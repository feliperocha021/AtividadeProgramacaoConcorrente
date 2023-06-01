#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TAM 50

int *CriarVetor(int n) {
  int *vetor;
  int i; 
  vetor = (int *)malloc(sizeof(int) * n);
  for (i=0;i<n;i++) {
    int num = rand() % 2;
    vetor[i] = num;
  }
  return vetor;
}

int main(){
    MPI_Init(NULL, NULL);

    int nprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *vetorPrincp = NULL, *subVetor = NULL, *vetorZero = NULL, *vetorUm = NULL;
    int qtdUm = 0, qtdZero = 0, i;
    int qtd = TAM/nprocs;
    subVetor = malloc(sizeof(int)*qtd);
    vetorZero = malloc(sizeof(int)*nprocs);
    vetorUm = malloc(sizeof(int)*nprocs);
    if(rank == 0){
        vetorPrincp = CriarVetor(TAM);
    }
    MPI_Scatter(vetorPrincp, qtd, MPI_INT, subVetor, qtd, MPI_INT, 0, MPI_COMM_WORLD);
    for (i = 0; i < qtd; i++) {
        if(subVetor[i] == 0) qtdZero += 1;
        else qtdUm += 1;    
    }
    MPI_Gather(&qtdUm, 1, MPI_INT, vetorUm, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&qtdZero, 1, MPI_INT, vetorZero, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int totalZero = 0, totalUm = 0;
        for (i = 0; i < nprocs; i++) {
            totalZero += vetorZero[i];
            totalUm += vetorUm[i];
        }
        printf("Número de zeros:%d  ", totalZero);
        printf("Número de uns:%d  ", totalUm);
        if ((totalUm % 2 )!= 0) {
            vetorPrincp =(int*)realloc(vetorPrincp, sizeof(int)*(TAM+1));
            vetorPrincp[TAM] = 1;
        }
        printf("\n");
        for (i = 0; i < TAM+1; i++) {
            printf("%d ", vetorPrincp[i]);
        }
    }
    printf("\n");
    MPI_Finalize();
    return 0;
}