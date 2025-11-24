#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura do Nó
typedef struct No {
    int valor;
    struct No* anterior;
    struct No* proximo;
} No;

// Protótipos das funções
No* criarNo(int valor);
void inserirInicio(No** cabeca, int valor);
void inserirFinal(No** cabeca, int valor);
void inserirPosicao(No** cabeca, int valor, int posicao);
void removerPosicao(No** cabeca, int posicao);
int buscarValor(No* cabeca, int valor);
void listar(No* cabeca);
void liberarMemoria(No** cabeca);

// Menu principal
int main() {
    No* lista = NULL;
    int opcao, valor, posicao, resultado;

    do {
        printf("\n=== MENU LISTA DUPLAMENTE ENCADEADA ===\n");
        printf("1. Inserir no Inicio\n");
        printf("2. Inserir no Final\n");
        printf("3. Inserir em Posicao Especifica\n");
        printf("4. Remover de Posicao Especifica\n");
        printf("5. Buscar Valor\n");
        printf("6. Listar Lista\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o valor: ");
                scanf("%d", &valor);
                inserirInicio(&lista, valor);
                break;
            case 2:
                printf("Digite o valor: ");
                scanf("%d", &valor);
                inserirFinal(&lista, valor);
                break;
            case 3:
                printf("Digite o valor: ");
                scanf("%d", &valor);
                printf("Digite a posicao (>=1): ");
                scanf("%d", &posicao);
                inserirPosicao(&lista, valor, posicao);
                break;
            case 4:
                printf("Digite a posicao para remover (>=1): ");
                scanf("%d", &posicao);
                removerPosicao(&lista, posicao);
                break;
            case 5:
                printf("Digite o valor a buscar: ");
                scanf("%d", &valor);
                resultado = buscarValor(lista, valor);
                if (resultado != -1) {
                    printf("Valor encontrado na posicao: %d\n", resultado);
                } else {
                    printf("Valor nao encontrado na lista.\n");
                }
                break;
            case 6:
                listar(lista);
                break;
            case 0:
                printf("Saindo e liberando memoria...\n");
                liberarMemoria(&lista);
                break;
            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}

// Função auxiliar para criar um novo nó
No* criarNo(int valor) {
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo) {
        novoNo->valor = valor;
        novoNo->anterior = NULL;
        novoNo->proximo = NULL;
    } else {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }
    return novoNo;
}

// 1. Inserir no início
void inserirInicio(No** cabeca, int valor) {
    No* novoNo = criarNo(valor);
    
    if (*cabeca != NULL) {
        novoNo->proximo = *cabeca;
        (*cabeca)->anterior = novoNo;
    }
    *cabeca = novoNo;
    printf("Valor %d inserido no inicio.\n", valor);
}

// 2. Inserir no final
void inserirFinal(No** cabeca, int valor) {
    No* novoNo = criarNo(valor);
    
    if (*cabeca == NULL) {
        *cabeca = novoNo;
    } else {
        No* temp = *cabeca;
        while (temp->proximo != NULL) {
            temp = temp->proximo;
        }
        temp->proximo = novoNo;
        novoNo->anterior = temp;
    }
    printf("Valor %d inserido no final.\n", valor);
}

// 3. Inserir em uma posição específica
void inserirPosicao(No** cabeca, int valor, int posicao) {
    if (posicao < 1) {
        printf("Posicao invalida!\n");
        return;
    }

    if (posicao == 1) {
        inserirInicio(cabeca, valor);
        return;
    }

    No* novoNo = criarNo(valor);
    No* temp = *cabeca;
    int contador = 1;

    // Percorre até encontrar a posição anterior à desejada
    while (contador < posicao - 1 && temp != NULL) {
        temp = temp->proximo;
        contador++;
    }

    if (temp == NULL) {
        printf("Posicao invalida! A lista e menor que a posicao solicitada.\n");
        free(novoNo); // Libera o nó criado inutilmente
    } else {
        // Inserção no meio ou logo após o último
        novoNo->proximo = temp->proximo;
        novoNo->anterior = temp;
        
        if (temp->proximo != NULL) {
            temp->proximo->anterior = novoNo;
        }
        temp->proximo = novoNo;
        printf("Valor %d inserido na posicao %d.\n", valor, posicao);
    }
}

// 4. Remover de uma posição específica
void removerPosicao(No** cabeca, int posicao) {
    if (*cabeca == NULL) {
        printf("A lista esta vazia.\n");
        return;
    }
    
    if (posicao < 1) {
        printf("Posicao invalida!\n");
        return;
    }

    No* temp = *cabeca;
    
    // Caso especial: remover o primeiro nó
    if (posicao == 1) {
        *cabeca = temp->proximo;
        if (*cabeca != NULL) {
            (*cabeca)->anterior = NULL;
        }
        free(temp);
        printf("No removido da posicao 1.\n");
        return;
    }

    // Navega até o nó a ser removido
    int contador = 1;
    while (contador < posicao && temp != NULL) {
        temp = temp->proximo;
        contador++;
    }

    if (temp == NULL) {
        printf("Posicao nao encontrada.\n");
    } else {
        // Ajusta os ponteiros do nó anterior e do próximo
        if (temp->anterior != NULL) {
            temp->anterior->proximo = temp->proximo;
        }
        if (temp->proximo != NULL) {
            temp->proximo->anterior = temp->anterior;
        }
        free(temp);
        printf("No removido da posicao %d.\n", posicao);
    }
}

// 5. Buscar valor
int buscarValor(No* cabeca, int valor) {
    int posicao = 1;
    No* temp = cabeca;
    
    while (temp != NULL) {
        if (temp->valor == valor) {
            return posicao;
        }
        temp = temp->proximo;
        posicao++;
    }
    return -1; // Não encontrado
}

// 6. Listar (Exibe ponteiros)
void listar(No* cabeca) {
    if (cabeca == NULL) {
        printf("Lista Vazia.\n");
        return;
    }

    printf("\n--- Conteudo da Lista Duplamente Encadeada ---\n");
    No* temp = cabeca;
    int pos = 1;
    
    while (temp != NULL) {
        printf("Pos: %d | Endereco: %p | Anterior: %p <- [ VALOR: %d ] -> Proximo: %p\n", 
               pos, (void*)temp, (void*)temp->anterior, temp->valor, (void*)temp->proximo);
        temp = temp->proximo;
        pos++;
    }
    printf("----------------------------------------------\n");
}

// Função para limpar a memória ao sair
void liberarMemoria(No** cabeca) {
    No* temp = *cabeca;
    while (temp != NULL) {
        No* remover = temp;
        temp = temp->proximo;
        free(remover);
    }
    *cabeca = NULL;
}
