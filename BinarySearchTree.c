/*
====================================================================
    UNIVERSIDADE FEDERAL DO AMAPÁ – IFAP
    Curso: Bacharelado em Ciência da Computação
    Disciplina: Programação II

    TRABALHO PRÁTICO – ÁRVORE BINÁRIA DE BUSCA (BST)

    Objetivo:
        Implementação de uma Árvore Binária de Busca com operações de:
        - Inserção de valores
        - Busca de valores
        - Remoção de nós
        - Percursos (pré-ordem, em ordem e pós-ordem)

    Integrantes do grupo:
        1 - Diego Maciel
        2 - Márleson Ferreira
        3 - Lucas
====================================================================
*/

#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura do Nó
typedef struct No
{
    int valor;
    struct No *esquerda;
    struct No *direita;
} No;

// --- Protótipos das Funções ---
No *criarNo(int valor);
No *inserir(No *raiz, int valor);
No *buscar(No *raiz, int valor);
No *encontrarMinimo(No *raiz);
No *remover(No *raiz, int valor);
void preOrdem(No *raiz);
void emOrdem(No *raiz);
void posOrdem(No *raiz);
void liberarArvore(No *raiz);
void menuPercorrer(No *raiz);

// --- Função Principal ---
int main()
{
    No *raiz = NULL; // Árvore começa vazia
    int opcao, valor;
    No *resultadoBusca;

    do
    {
        printf("\n=== MENU PRINCIPAL BST ===\n");
        printf("1 - Inserir valor\n");
        printf("2 - Buscar valor\n");
        printf("3 - Remover valor\n");
        printf("4 - Percorrer arvore\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Digite o valor para inserir: ");
            scanf("%d", &valor);
            raiz = inserir(raiz, valor);
            printf("Valor inserido (ou verificado duplicidade).\n");
            break;

        case 2:
            printf("Digite o valor para buscar: ");
            scanf("%d", &valor);
            resultadoBusca = buscar(raiz, valor);
            if (resultadoBusca != NULL)
            {
                printf("\nValor %d ENCONTRADO na arvore.\n", valor);
            }
            else
            {
                printf("\nValor %d NAO encontrado.\n", valor);
            }
            break;

        case 3:
            printf("Digite o valor para remover: ");
            scanf("%d", &valor);
            raiz = remover(raiz, valor);
            printf("Operacao de remocao concluida.\n");
            break;

        case 4:
            menuPercorrer(raiz);
            break;

        case 0:
            printf("Liberando memoria e saindo...\n");
            liberarArvore(raiz);
            printf("Programa encerrado.\n");
            break;

        default:
            printf("Opcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// --- Implementação das Funções ---

// Função auxiliar para alocar memória e criar um novo nó
No *criarNo(int valor)
{
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL)
    {
        printf("Erro de alocacao de memoria!\n");
        exit(1);
    }
    novo->valor = valor;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// 1. Inserção
// Insere um valor respeitando a regra da BST (menores à esquerda, maiores à direita)
No *inserir(No *raiz, int valor)
{
    // Se a árvore (ou subárvore) estiver vazia, cria o nó aqui
    if (raiz == NULL)
    {
        return criarNo(valor);
    }

    // Navegação recursiva
    if (valor < raiz->valor)
    {
        raiz->esquerda = inserir(raiz->esquerda, valor);
    }
    else if (valor > raiz->valor)
    {
        raiz->direita = inserir(raiz->direita, valor);
    }
    else
    {
        // Valores repetidos são ignorados nesta implementação
        printf("\n[AVISO] O valor %d ja existe na arvore.\n", valor);
    }
    return raiz;
}

// 2. Busca
// Retorna o ponteiro para o nó se encontrar, ou NULL se não encontrar
No *buscar(No *raiz, int valor)
{
    if (raiz == NULL || raiz->valor == valor)
    {
        return raiz;
    }

    if (valor < raiz->valor)
    {
        return buscar(raiz->esquerda, valor);
    }
    else
    {
        return buscar(raiz->direita, valor);
    }
}

// Função auxiliar para a remoção: encontra o menor valor de uma subárvore
No *encontrarMinimo(No *raiz)
{
    No *atual = raiz;
    // O menor valor sempre está no nó mais à esquerda
    while (atual && atual->esquerda != NULL)
    {
        atual = atual->esquerda;
    }
    return atual;
}

// 3. Remoção
// Trata os três casos de remoção de nós em uma BST
No *remover(No *raiz, int valor)
{
    if (raiz == NULL)
    {
        printf("\nValor nao encontrado para remocao.\n");
        return raiz;
    }

    // Navega até encontrar o nó
    if (valor < raiz->valor)
    {
        raiz->esquerda = remover(raiz->esquerda, valor);
    }
    else if (valor > raiz->valor)
    {
        raiz->direita = remover(raiz->direita, valor);
    }
    else
    {
        // Encontrou o nó a ser removido (raiz->valor == valor)

        // CASO 1 e 2: Nó sem filhos (folha) ou com apenas um filho
        if (raiz->esquerda == NULL)
        {
            No *temp = raiz->direita;
            free(raiz);  // Libera a memória do nó atual
            return temp; // Retorna o filho da direita (pode ser NULL) para o pai ligar
        }
        else if (raiz->direita == NULL)
        {
            No *temp = raiz->esquerda;
            free(raiz);  // Libera a memória do nó atual
            return temp; // Retorna o filho da esquerda para o pai ligar
        }

        // CASO 3: Nó com dois filhos
        // Estratégia: Substituir pelo Sucessor In-Order (o menor valor da subárvore direita)
        No *temp = encontrarMinimo(raiz->direita);

        // Copia o valor do sucessor para o nó atual
        raiz->valor = temp->valor;

        // Remove o sucessor da subárvore direita (agora recursivamente cairá no caso 1 ou 2 lá embaixo)
        raiz->direita = remover(raiz->direita, temp->valor);
    }
    return raiz;
}

// 4. Percursos

// Pré-ordem: Raiz -> Esquerda -> Direita
void preOrdem(No *raiz)
{
    if (raiz != NULL)
    {
        printf("%d ", raiz->valor);
        preOrdem(raiz->esquerda);
        preOrdem(raiz->direita);
    }
}

// Em ordem: Esquerda -> Raiz -> Direita (Imprime valores ordenados)
void emOrdem(No *raiz)
{
    if (raiz != NULL)
    {
        emOrdem(raiz->esquerda);
        printf("%d ", raiz->valor);
        emOrdem(raiz->direita);
    }
}

// Pós-ordem: Esquerda -> Direita -> Raiz
void posOrdem(No *raiz)
{
    if (raiz != NULL)
    {
        posOrdem(raiz->esquerda);
        posOrdem(raiz->direita);
        printf("%d ", raiz->valor);
    }
}

// Função para liberar toda a memória (Pós-ordem é ideal para isso)
void liberarArvore(No *raiz)
{
    if (raiz != NULL)
    {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

// Submenu para os percursos
void menuPercorrer(No *raiz)
{
    int op;
    if (raiz == NULL)
    {
        printf("\nArvore vazia.\n");
        return;
    }

    printf("\n--- SUBMENU DE PERCURSO ---\n");
    printf("1 - Pre-ordem\n");
    printf("2 - Em ordem\n");
    printf("3 - Pos-ordem\n");
    printf("Escolha: ");
    scanf("%d", &op);

    printf("\nResultado: ");
    switch (op)
    {
    case 1:
        preOrdem(raiz);
        break;
    case 2:
        emOrdem(raiz);
        break;
    case 3:
        posOrdem(raiz);
        break;
    default:
        printf("Opcao invalida.");
    }
    printf("\n");
}
