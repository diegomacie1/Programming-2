#include <stdio.h>
#include <stdlib.h>

void counting_sort(int *A, int n)
{
    if (n <= 1)
        return;

    // 1) achar min e max
    int min = A[0], max = A[0];
    for (int i = 1; i < n; i++)
    {
        if (A[i] < min)
            min = A[i];
        if (A[i] > max)
            max = A[i];
    }

    int k = max - min + 1;

    // 2) vetor de contagem C (inicializado com zero)
    int *C = (int *)calloc(k, sizeof(int));
    if (!C)
    {
        perror("calloc");
        exit(1);
    }

    // 3) contar ocorrências
    for (int i = 0; i < n; i++)
    {
        C[A[i] - min]++;
    }

    // 4) prefix sum (acumulada)
    for (int i = 1; i < k; i++)
    {
        C[i] += C[i - 1];
    }

    // 5) vetor auxiliar B
    int *B = (int *)malloc(n * sizeof(int));
    if (!B)
    {
        perror("malloc");
        free(C);
        exit(1);
    }

    // 6) preencher B de trás pra frente (estável)
    for (int i = n - 1; i >= 0; i--)
    {
        int idx = A[i] - min;
        int pos = C[idx] - 1;
        B[pos] = A[i];
        C[idx]--;
    }

    // 7) copiar de volta para A
    for (int i = 0; i < n; i++)
    {
        A[i] = B[i];
    }

    free(B);
    free(C);
}

int main(void)
{
    int A[] = {4, 2, 2, -8, 3, -3, 1};
    int n = (int)(sizeof(A) / sizeof(A[0]));

    counting_sort(A, n);

    printf("\nArray depois do Counting Sort: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", A[i]);
    }
    printf("\n");
    return 0;
}
