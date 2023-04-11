
#include <omp.h>
#include <stdio.h>
#include <time.h>

#define TAMANHO 100000000

int main() {
    int i; double somaSerial = 0.0, somaParalela = 0.0;
    int t = omp_get_num_threads();

    //Execução Sequencial
    double inicio = omp_get_wtime();
    for(i = 1; i <= TAMANHO; i++) {
        if(TAMANHO % i == 0) {
            somaSerial += i;
        }
    }
    double fim = omp_get_wtime();
    double tSerial = fim - inicio;
    printf("Execução Sequencial: %fseg\n", tSerial);
    printf("Soma Serial: %f\n", somaSerial);

    //Execução Paralela
    inicio = omp_get_wtime();
    double somaParalela2 = 0.0;
    #pragma omp parallel private(somaParalela)num_threads(3)
    {
        #pragma omp for
            for(i = 1; i <= TAMANHO; i++) {
                if(TAMANHO % i == 0) {
                    somaParalela += i;
                }
            }
        #pragma omp critical
            somaParalela2 += somaParalela;
    }
    fim = omp_get_wtime();
    double tParalelo = fim - inicio;
    double speedup = tSerial/tParalelo;
    printf("Soma Paralela: %f\n",somaParalela2);
    printf("Execucao paralela: %fseg\n",tParalelo);
    printf("Speedup: %f\n", speedup);
    printf("Eficiencia: %f\n",speedup/3.0);
    printf("\n");

    //Execução Paralela Static
    inicio = omp_get_wtime();
    double somaStatic = 0.0;
    #pragma omp parallel num_threads(3)
    {
        double soma = 0.0;
        #pragma omp for schedule(static, 3)
            for(i = 1; i <= TAMANHO; i++) {
                if(TAMANHO % i == 0) {
                    soma += i;
                }
            }
        #pragma omp critical
            somaStatic += soma;
    }
    fim = omp_get_wtime();
    double tParaleloStatic = fim - inicio;
    speedup = tSerial/tParaleloStatic;
    printf("Soma ParalelaStatic: %f\n",somaStatic);
    printf("Execucao paralelastatic: %fseg\n",tParaleloStatic);
    printf("Speedup: %f\n", speedup);
    printf("Eficiencia: %f\n",speedup/3.0);
    printf("\n");
    
    //Execução Paralela Dynamic
    inicio = omp_get_wtime();
    double somaDynamic = 0.0;
    #pragma omp parallel num_threads(3)
    {
        double soma = 0.0;
        #pragma omp for schedule(dynamic, 3)
            for(i = 1; i <= TAMANHO; i++) {
                if(TAMANHO % i == 0) {
                    soma += i;
                }
            }
        #pragma omp critical
            somaDynamic += soma;
    }
    fim = omp_get_wtime();
    double tParaleloDynamic = fim - inicio;
    speedup = tSerial/tParaleloDynamic;
    printf("Soma ParalelaDynamic: %f\n",somaDynamic);
    printf("Execucao paraleladynamic: %fseg\n",tParaleloDynamic);
    printf("Speedup: %f\n", speedup);
    printf("Eficiencia: %f\n",speedup/3.0);
    return 0;
}