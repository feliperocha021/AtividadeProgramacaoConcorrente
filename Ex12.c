#include <stdio.h>
#include <mpi.h>

#define NUMSTEPS 1000000

int main() {
    MPI_Init(NULL, NULL);
    int nprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double step;
    int i; double x, pi, soma = 0.0;

    step = 1.0 / NUMSTEPS;
    for(i = rank; i < NUMSTEPS; i += nprocs) {
        x = (i + 0.5) * step;
        soma = soma + 4.0 / (1.0 + x * x);
    }
    pi = step * soma;
    double piTotal;
    MPI_Reduce(&pi, &piTotal, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if(rank == 0) {
        printf("%f\n", piTotal);
    }
    MPI_Finalize();
    return 0;
}
