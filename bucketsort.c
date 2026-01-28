#include <stdio.h>
#include <stdlib.h>

// Exemplo para valores entre 0.0 e 1.0
void bucketSort(float arr[], int tamanho) {

    // Cria 10 baldes
    int num_baldes = 10;
    float baldes[num_baldes][tamanho]; 
    int qtd_por_balde[num_baldes];
    
    for (int i = 0; i < num_baldes; i++) qtd_por_balde[i] = 0;

    // Distribui os elementos nos baldes
    for (int i = 0; i < tamanho; i++) {
        int indice_balde = num_baldes * arr[i]; 
        baldes[indice_balde][qtd_por_balde[indice_balde]++] = arr[i];
    }

    // Ordena cada balde e junta tudo
    int indice_original = 0;
    for (int i = 0; i < num_baldes; i++) {
        // Ordenação por inserção dentro do balde
        for (int j = 1; j < qtd_por_balde[i]; j++) {
            float chave = baldes[i][j];
            int k = j - 1;
            while (k >= 0 && baldes[i][k] > chave) {
                baldes[i][k + 1] = baldes[i][k];
                k--;
            }
            baldes[i][k + 1] = chave;
        }
        // Devolve para o array original
        for (int j = 0; j < qtd_por_balde[i]; j++) {
            arr[indice_original++] = baldes[i][j];
        }
    }
}