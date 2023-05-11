#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO 1000

int main() {
    MPI_Init(NULL, NULL);
    int nprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        
        long int somaTotal,somaTotalDobro;
        long int *vetor;
        vetor = malloc(TAMANHO*sizeof(long int));
        time_t t;
        srand((unsigned) time(&t));
        for (int i = 0;i < TAMANHO; i++) {
            long int num = (rand() % (TAMANHO+1));
            vetor[i] = num;
        }
        MPI_Bcast(vetor,TAMANHO,MPI_LONG,0,MPI_COMM_WORLD);
        MPI_Recv(&somaTotal, 1, MPI_LONG, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&somaTotalDobro, 1, MPI_LONG, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Total: %ld\n",somaTotal);
        printf("Total: %ld\n",somaTotalDobro);
   } else {
        long int *vetor;
        vetor = malloc(TAMANHO*sizeof(long int));
        MPI_Bcast(vetor,TAMANHO,MPI_LONG,0,MPI_COMM_WORLD);
        long int soma = 0;
        for (int i = 0; i < TAMANHO; i++) {
            if (rank == 1) soma = soma + vetor[i];
            else if (rank == 2) soma = soma + (2 * vetor[i]);
        }
        MPI_Send(&soma, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);
   }
    MPI_Finalize();
    return 0;
}