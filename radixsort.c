#include <stdio.h>

// Função auxiliar para fazer o counting sort baseado no dígito exp (1, 10, 100...)
void countingSortParaRadix(int arr[], int tamanho, int exp) {
    int saida[tamanho];
    int contagem[10] = {0};

    // Preenche a contagem de cada dígito (0 a 9)
    for (int i = 0; i < tamanho; i++){
        int digito = (arr[i] / exp) % 10; // Extrai o digito específico da posição que estamos ordenando
        contagem[digito]++;
    }

    // Transforma a contagem em índices reais (Acumulada)
    for (int i = 1; i < 10; i++){
        contagem[i] += contagem[i - 1];
    }

    // Monta o array de saída (Percorrendo de trás para frente para manter estabilidade)
    for (int i = tamanho - 1; i >= 0; i--){
        int digito = (arr[i] / exp) % 10; // Extrai o digito específico da posição que estamos ordenando
        saida[contagem[digito] - 1] = arr[i];
        contagem[digito]--; // Decrementa a posição do dígito
    }

    // Copia a saída de volta para o array original
    for (int i = 0; i < tamanho; i++){
        arr[i] = saida[i];
    }
}

void radixSort(int arr[], int tamanho) {
    // Acha o maior número para saber quantos dígitos processar
    int maior = arr[0];
    for (int i = 1; i < tamanho; i++){
        if (arr[i] > maior){
            maior = arr[i];
        }
    }

    // Chama a função para cada casa decimal (1, 10, 100...)
    for (int expoente = 1; (maior / expoente) > 0; expoente *= 10){
        countingSortParaRadix(arr, tamanho, expoente);
    }
}

// Função para imprimir o array
void imprimirArray(int arr[], int tamanho) {
    for (int i = 0; i < tamanho; i++){
        printf("%d ", arr[i]);
    }
    printf("\n\n");
}

int main() {
    int dados[] = {170, 45, 75, 90, 802, 24, 2, 66};
    int n = sizeof(dados) / sizeof(dados[0]); // retorna o número de elementos no array

    printf("Array original:\n");

    imprimirArray(dados, n);

    radixSort(dados, n);

    printf("Array ordenado com Radix Sort:\n");
    imprimirArray(dados, n);

    return 0;
}
