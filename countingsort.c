#include <stdio.h>

void countingSort(int arr[], int tamanho) {
    // Encontra o maior valor para saber o tamanho do array de contagem
    int maior = arr[0];
    for (int i = 1; i < tamanho; i++) {
        if (arr[i] > maior) maior = arr[i];
    }

    // Cria o array de contagem (VLA) e inicializa com zeros
    // Importante: funciona apenas para números não-negativos
    int contagem[maior + 1];
    for (int i = 0; i <= maior; ++i) contagem[i] = 0;

    // Frequência: Conta quantas vezes cada número aparece
    for (int i = 0; i < tamanho; i++) {
        contagem[arr[i]]++;
    }

    // Reescrita: Coloca os valores de volta no array original em ordem
    int indice_atual = 0;
    for (int i = 0; i <= maior; i++) {
        while (contagem[i] > 0) {
            arr[indice_atual++] = i;
            contagem[i]--;
        }
    }
}

int main() {
    // Exemplo de teste
    int meu_array[] = {4, 2, 2, 8, 3, 3, 1};
    int n = sizeof(meu_array) / sizeof(meu_array[0]);

    printf("Array antes da ordenacao: ");
    for(int i = 0; i < n; i++) printf("%d ", meu_array[i]);

    countingSort(meu_array, n);

    printf("\nArray depois do Counting Sort: ");
    for(int i = 0; i < n; i++) printf("%d ", meu_array[i]);
    printf("\n");

    return 0;
}
