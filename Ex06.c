#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define NUMSTEPS 1000000000

int main() {
    double step;
    int i; double x, pi, somaSerial = 0.0, somaParalela = 0.0;
    int t = omp_get_num_threads();

    //Execução sequencial
    double inicio = omp_get_wtime();
    step = 1.0 / NUMSTEPS;
    for(i = 0; i < NUMSTEPS; i++) {
        x = (i + 0.5) * step;
        somaSerial = somaSerial + 4.0 / (1.0 + x * x);
    }
    pi = step * somaSerial;
    double fim = omp_get_wtime();
    double tSerial = fim - inicio;
    printf("Execucao Sequencial: %fseg\n", tSerial);
    printf("Pi: %f\n", pi);

    //Execução paralela
    x = 0;
    pi = 0;
    inicio = omp_get_wtime();
    step = 1.0 / NUMSTEPS;
    #pragma omp parallel private(somaParalela, x)num_threads(3)
    {
        #pragma omp for
            for(i = 0; i < NUMSTEPS; i++) {
                x = (i + 0.5) * step;
                somaParalela = somaParalela + 4.0 / (1.0 + x * x); 
            }
        #pragma omp critical
            pi += somaParalela * step;
    }
    fim = omp_get_wtime();
    double tParalelo = fim - inicio;
    double speedup = tSerial/tParalelo;
    printf("Pi: %f\n",pi);
    printf("Execucao paralela: %fseg\n",tParalelo);
    printf("Speedup: %f\n", speedup);
    printf("Eficiencia: %f\n",speedup/3.0);
    return 0;
}