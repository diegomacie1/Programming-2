#include <stdio.h>

// Função auxiliar para fazer o counting sort baseado no dígito exp (10^i)
void countingSortParaRadix(int arr[], int tamanho, int exp) {
    int saida[tamanho];
    int contagem[10] = {0};

    // Calcula a frequência baseada no dígito atual
    for (int i = 0; i < tamanho; i++)
        contagem[(arr[i] / exp) % 10]++;

    // Transforma a contagem em posições reais no array de saída
    for (int i = 1; i < 10; i++)
        contagem[i] += contagem[i - 1];

    // Monta o array de saída (de trás para frente para manter estabilidade)
    for (int i = tamanho - 1; i >= 0; i--) {
        saida[contagem[(arr[i] / exp) % 10] - 1] = arr[i];
        contagem[(arr[i] / exp) % 10]--;
    }

    for (int i = 0; i < tamanho; i++)
        arr[i] = saida[i];
}

void radixSort(int arr[], int tamanho) {
    int maior = arr[0];
    for (int i = 1; i < tamanho; i++)
        if (arr[i] > maior) maior = arr[i];

    // Percorre cada dígito (1, 10, 100...)
    for (int expoente = 1; maior / expoente > 0; expoente *= 10)
        countingSortParaRadix(arr, tamanho, expoente);
}