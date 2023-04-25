#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define TAMANHO 100000000

float raiz_quadrada (double numero)
{
    int i;
    double recorre = numero;
    
    for (i = 0; i < 10; i++)
          recorre = recorre/2 + numero/(2*recorre);
            
    return(recorre);    
}    

int main() {
    time_t t;
    srand((unsigned) time(&t));

    float *v;
    v = (float*)malloc(TAMANHO * sizeof(float));
    int i;
    for(i = 0; i < TAMANHO; i++) {
        int num = (float) rand() / RAND_MAX;
        v[i] = num;
    }

    //Execução Serial

    float somaSerial = 0;
    double inicio = omp_get_wtime();
    for(i = 0; i < TAMANHO; i++) {
        somaSerial += v[i];
    }
    float media = somaSerial / TAMANHO;

    somaSerial = 0;
    for(i = 0; i < TAMANHO; i++) {
        somaSerial = somaSerial + (v[i] - media) * (v[i] - media);
    }

    float dp_serial = raiz_quadrada(somaSerial / TAMANHO);

    double fim = omp_get_wtime();
    double t_serial = fim - inicio;
    
    printf("Tempo execucao Sequencial: %fseg\n", t_serial);
    printf("Desvio padrão: %f\n", dp_serial);

    //Execução Paralela
    float somaParalela = 0;
    float mediaParalela = 0;
    inicio = omp_get_wtime();
    #pragma omp parallel num_threads(3)
    {
        float somaThread = 0;
        #pragma omp for
            for(i = 0; i < TAMANHO; i++) {
                somaThread += v[i];
            }
        #pragma omp critical
            mediaParalela += somaThread / TAMANHO;

        somaThread = 0;
        #pragma omp for
            for(i = 0; i < TAMANHO; i++) {
                somaThread = somaThread + (v[i] - mediaParalela) * (v[i] - mediaParalela);
            }
        #pragma omp critical
            somaParalela += somaThread;
    }
    float dp_paralelo = raiz_quadrada(somaParalela / TAMANHO);

    fim = omp_get_wtime();
    double t_paralelo = fim - inicio;
    double speedup = t_serial / t_paralelo;
    printf("Tempo execucao Paralela: %fseg\n", t_paralelo);
    printf("Desvio padrão: %f\n", dp_paralelo);
    printf("Speedup: %f\n", speedup);
    printf("Eficiencia: %f\n",speedup/3.0);
    return 0;
}