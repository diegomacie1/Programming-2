#include <stdio.h>

void bucketSort(float arr[], int tamanho) {
    int num_baldes = 10;
    // Criamos a matriz de baldes
    float baldes[10][tamanho]; 
    int qtd_por_balde[10];
    
    // Inicializa os contadores de cada balde com zero
    for (int i = 0; i < num_baldes; i++) qtd_por_balde[i] = 0;

    // Distribui os elementos nos baldes
    for (int i = 0; i < tamanho; i++) {
        int indice_balde = (int)(num_baldes * arr[i]); 
        
        // Proteção para não estourar os índices (limita entre 0 e 9)
        if (indice_balde >= num_baldes) indice_balde = num_baldes - 1;
        if (indice_balde < 0) indice_balde = 0;

        baldes[indice_balde][qtd_por_balde[indice_balde]++] = arr[i];
    }

    // Ordena cada balde e junta tudo
    int indice_original = 0;
    // Para ordenação em ordem decrescente -> (int i = num_baldes - 1; i >= 0; i--)
    for (int i = 0; i < num_baldes; i++) {
        // Ordenação por inserção (Insertion Sort) dentro do balde
        for (int j = 1; j < qtd_por_balde[i]; j++) {
            float chave = baldes[i][j];
            int k = j - 1;
            // Para ordenação em ordem decrescente -> (k >= 0 && baldes[i][k] < chave)
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

int main() {
    // Exemplo com números decimais entre 0 e 1
    float dados[] = {0.78, 0.17, 0.39, 0.26, 0.72, 0.94, 0.21, 0.12, 0.23, 0.68};
    int n = sizeof(dados) / sizeof(dados[0]);

    printf("Array original:\n");
    for (int i = 0; i < n; i++) printf("%.2f ", dados[i]);
    printf("\n\n");

    bucketSort(dados, n);

    printf("Array ordenado (Bucket Sort):\n");
    for (int i = 0; i < n; i++) printf("%.2f ", dados[i]);
    printf("\n");

    return 0;
}
