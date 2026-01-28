#include <stdio.h>

void countingSort(int arr[], int tamanho) {
    int maior = arr[0];
    for (int i = 1; i < tamanho; i++) {
        if (arr[i] > maior) maior = arr[i];
    }

    int contagem[maior + 1];
    for (int i = 0; i <= maior; ++i) contagem[i] = 0;

    // Conta a ocorrência de cada elemento
    for (int i = 0; i < tamanho; i++) contagem[arr[i]]++;

    // Reconstrói o array original de forma ordenada
    int indice_atual = 0;
    for (int i = 0; i <= maior; i++) {
        while (contagem[i] > 0) {
            arr[indice_atual++] = i;
            contagem[i]--;
        }
    }
}