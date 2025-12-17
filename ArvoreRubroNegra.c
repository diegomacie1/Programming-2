/*
====================================================================
    UNIVERSIDADE FEDERAL DO AMAPÁ – UNIFAP
    Curso: Bacharelado em Ciência da Computação
    Disciplina: Programação II

    TRABALHO PRÁTICO – ÁRVORE RUBRO NEGRA

    Objetivo:
        Implementação de uma Árvore Rubro Negra com operações de:
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

// Definição das cores
enum Cor { VERMELHO, PRETO };

// Estrutura do Nó (No)
typedef struct No {
    int valor;
    enum Cor cor;
    struct No *esquerda, *direita, *pai;
} No;

// Estrutura da Árvore (ArvoreRB)
typedef struct ArvoreRB {
    No *raiz;
    No *TNULL; // Sentinela (Substitui o NULL)
} ArvoreRB;

// --- Protótipos das Funções ---
ArvoreRB* criarArvore();
void rotacaoEsquerda(ArvoreRB *arvore, No *x);
void rotacaoDireita(ArvoreRB *arvore, No *x);
void inserir(ArvoreRB *arvore, int valor);
void balancearInsercao(ArvoreRB *arvore, No *k);
void remover(ArvoreRB *arvore, int valor);
void balancearRemocao(ArvoreRB *arvore, No *x);
void transplantar(ArvoreRB *arvore, No *u, No *v);
No* minimo(ArvoreRB *arvore, No *no);
No* buscar(ArvoreRB *arvore, No *no, int valor);
void preOrdem(ArvoreRB *arvore, No *no);
void emOrdem(ArvoreRB *arvore, No *no);
void posOrdem(ArvoreRB *arvore, No *no);
void menu(ArvoreRB *arvore);

// --- Implementação ---

// Inicializa a árvore e o nó Sentinela
ArvoreRB* criarArvore() {
    ArvoreRB *arvore = (ArvoreRB*)malloc(sizeof(ArvoreRB));
    
    // Aloca memória para o TNULL
    arvore->TNULL = (No*)malloc(sizeof(No));
    arvore->TNULL->cor = PRETO;
    arvore->TNULL->esquerda = NULL;
    arvore->TNULL->direita = NULL;
    
    // A raiz aponta inicialmente para TNULL
    arvore->raiz = arvore->TNULL;
    return arvore;
}

// Rotação para a Esquerda
void rotacaoEsquerda(ArvoreRB *arvore, No *x) {
    No *y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != arvore->TNULL) {
        y->esquerda->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == NULL) {
        arvore->raiz = y;
    } else if (x == x->pai->esquerda) {
        x->pai->esquerda = y;
    } else {
        x->pai->direita = y;
    }
    y->esquerda = x;
    x->pai = y;
}

// Rotação para a Direita
void rotacaoDireita(ArvoreRB *arvore, No *x) {
    No *y = x->esquerda;
    x->esquerda = y->direita;
    if (y->direita != arvore->TNULL) {
        y->direita->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == NULL) {
        arvore->raiz = y;
    } else if (x == x->pai->direita) {
        x->pai->direita = y;
    } else {
        x->pai->esquerda = y;
    }
    y->direita = x;
    x->pai = y;
}

// Correção após Inserção (Balanceamento)
void balancearInsercao(ArvoreRB *arvore, No *k) {
    No *tio;
    while (k->pai->cor == VERMELHO) {
        if (k->pai == k->pai->pai->direita) {
            tio = k->pai->pai->esquerda; 
            if (tio->cor == VERMELHO) {
                // Caso 1: Tio é vermelho
                tio->cor = PRETO;
                k->pai->cor = PRETO;
                k->pai->pai->cor = VERMELHO;
                k = k->pai->pai;
            } else {
                if (k == k->pai->esquerda) {
                    // Caso 2: Tio é preto e k é filho à esquerda (Triângulo)
                    k = k->pai;
                    rotacaoDireita(arvore, k);
                }
                // Caso 3: Tio é preto e k é filho à direita (Linha)
                k->pai->cor = PRETO;
                k->pai->pai->cor = VERMELHO;
                rotacaoEsquerda(arvore, k->pai->pai);
            }
        } else {
            tio = k->pai->pai->direita; 
            if (tio->cor == VERMELHO) {
                // Caso 1 espelhado
                tio->cor = PRETO;
                k->pai->cor = PRETO;
                k->pai->pai->cor = VERMELHO;
                k = k->pai->pai;
            } else {
                if (k == k->pai->direita) {
                    // Caso 2 espelhado
                    k = k->pai;
                    rotacaoEsquerda(arvore, k);
                }
                // Caso 3 espelhado
                k->pai->cor = PRETO;
                k->pai->pai->cor = VERMELHO;
                rotacaoDireita(arvore, k->pai->pai);
            }
        }
        if (k == arvore->raiz) break;
    }
    arvore->raiz->cor = PRETO;
}

// Inserção Principal
void inserir(ArvoreRB *arvore, int valor) {
    No *novo = (No*)malloc(sizeof(No));
    novo->pai = NULL;
    novo->valor = valor;
    novo->esquerda = arvore->TNULL;
    novo->direita = arvore->TNULL;
    novo->cor = VERMELHO; // Novos nós são sempre vermelhos

    No *y = NULL;
    No *x = arvore->raiz;

    while (x != arvore->TNULL) {
        y = x;
        if (novo->valor < x->valor) {
            x = x->esquerda;
        } else {
            x = x->direita;
        }
    }

    novo->pai = y;
    if (y == NULL) {
        arvore->raiz = novo;
    } else if (novo->valor < y->valor) {
        y->esquerda = novo;
    } else {
        y->direita = novo;
    }

    if (novo->pai == NULL) {
        novo->cor = PRETO;
        return;
    }

    if (novo->pai->pai == NULL) {
        return;
    }

    balancearInsercao(arvore, novo);
}

// Auxiliar: Transplante de nós (usado na remoção)
void transplantar(ArvoreRB *arvore, No *u, No *v) {
    if (u->pai == NULL) {
        arvore->raiz = v;
    } else if (u == u->pai->esquerda) {
        u->pai->esquerda = v;
    } else {
        u->pai->direita = v;
    }
    v->pai = u->pai;
}

// Auxiliar: Encontrar mínimo
No* minimo(ArvoreRB *arvore, No *no) {
    while (no->esquerda != arvore->TNULL) {
        no = no->esquerda;
    }
    return no;
}

// Correção após Remoção (O caso mais complexo da RB Tree)
void balancearRemocao(ArvoreRB *arvore, No *x) {
    No *irmao;
    while (x != arvore->raiz && x->cor == PRETO) {
        if (x == x->pai->esquerda) {
            irmao = x->pai->direita;
            // Caso 1: Irmão é vermelho
            if (irmao->cor == VERMELHO) {
                irmao->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoEsquerda(arvore, x->pai);
                irmao = x->pai->direita;
            }
            // Caso 2: Irmão é preto e ambos sobrinhos são pretos
            if (irmao->esquerda->cor == PRETO && irmao->direita->cor == PRETO) {
                irmao->cor = VERMELHO;
                x = x->pai;
            } else {
                // Caso 3: Irmão preto, sobrinho esquerdo vermelho, direito preto
                if (irmao->direita->cor == PRETO) {
                    irmao->esquerda->cor = PRETO;
                    irmao->cor = VERMELHO;
                    rotacaoDireita(arvore, irmao);
                    irmao = x->pai->direita;
                }
                // Caso 4: Irmão preto, sobrinho direito vermelho
                irmao->cor = x->pai->cor;
                x->pai->cor = PRETO;
                irmao->direita->cor = PRETO;
                rotacaoEsquerda(arvore, x->pai);
                x = arvore->raiz;
            }
        } else { // Simétrico (espelhado)
            irmao = x->pai->esquerda;
            if (irmao->cor == VERMELHO) {
                irmao->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoDireita(arvore, x->pai);
                irmao = x->pai->esquerda;
            }
            if (irmao->direita->cor == PRETO && irmao->esquerda->cor == PRETO) {
                irmao->cor = VERMELHO;
                x = x->pai;
            } else {
                if (irmao->esquerda->cor == PRETO) {
                    irmao->direita->cor = PRETO;
                    irmao->cor = VERMELHO;
                    rotacaoEsquerda(arvore, irmao);
                    irmao = x->pai->esquerda;
                }
                irmao->cor = x->pai->cor;
                x->pai->cor = PRETO;
                irmao->esquerda->cor = PRETO;
                rotacaoDireita(arvore, x->pai);
                x = arvore->raiz;
            }
        }
    }
    x->cor = PRETO;
}

// Remoção Principal
void remover(ArvoreRB *arvore, int valor) {
    No *z = arvore->TNULL;
    No *x, *y;
    No *temp = arvore->raiz;

    // Buscar o nó a ser removido
    while (temp != arvore->TNULL) {
        if (temp->valor == valor) {
            z = temp;
            break;
        }
        if (temp->valor <= valor) {
            temp = temp->direita;
        } else {
            temp = temp->esquerda;
        }
    }

    if (z == arvore->TNULL) {
        printf("Valor %d nao encontrado na arvore.\n", valor);
        return;
    }

    y = z;
    int cor_original_y = y->cor;

    if (z->esquerda == arvore->TNULL) {
        x = z->direita;
        transplantar(arvore, z, z->direita);
    } else if (z->direita == arvore->TNULL) {
        x = z->esquerda;
        transplantar(arvore, z, z->esquerda);
    } else {
        y = minimo(arvore, z->direita);
        cor_original_y = y->cor;
        x = y->direita;
        if (y->pai == z) {
            x->pai = y;
        } else {
            transplantar(arvore, y, y->direita);
            y->direita = z->direita;
            y->direita->pai = y;
        }
        transplantar(arvore, z, y);
        y->esquerda = z->esquerda;
        y->esquerda->pai = y;
        y->cor = z->cor;
    }
    
    free(z); // Libera memória do nó removido

    if (cor_original_y == PRETO) {
        balancearRemocao(arvore, x);
    }
    printf("Valor %d removido com sucesso.\n", valor);
}

// Busca Simples
No* buscar(ArvoreRB *arvore, No *no, int valor) {
    if (no == arvore->TNULL || valor == no->valor) {
        return no;
    }
    if (valor < no->valor) {
        return buscar(arvore, no->esquerda, valor);
    }
    return buscar(arvore, no->direita, valor);
}

// Impressão (V = Vermelho, P = Preto)
void preOrdem(ArvoreRB *arvore, No *no) {
    if (no != arvore->TNULL) {
        printf("%d(%c) ", no->valor, (no->cor == VERMELHO) ? 'V' : 'P');
        preOrdem(arvore, no->esquerda);
        preOrdem(arvore, no->direita);
    }
}

void emOrdem(ArvoreRB *arvore, No *no) {
    if (no != arvore->TNULL) {
        emOrdem(arvore, no->esquerda);
        printf("%d(%c) ", no->valor, (no->cor == VERMELHO) ? 'V' : 'P');
        emOrdem(arvore, no->direita);
    }
}

void posOrdem(ArvoreRB *arvore, No *no) {
    if (no != arvore->TNULL) {
        posOrdem(arvore, no->esquerda);
        posOrdem(arvore, no->direita);
        printf("%d(%c) ", no->valor, (no->cor == VERMELHO) ? 'V' : 'P');
    }
}

// Menu Interativo
void menu(ArvoreRB *arvore) {
    int opcao, valor;
    No* res;
    
    do {
        printf("\n\n=== ARVORE RUBRO-NEGRA ===\n");
        printf("1. Inserir\n");
        printf("2. Remover\n");
        printf("3. Buscar\n");
        printf("4. Exibir Pre-Ordem\n");
        printf("5. Exibir Em-Ordem\n");
        printf("6. Exibir Pos-Ordem\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o valor para inserir: ");
                scanf("%d", &valor);
                inserir(arvore, valor);
                break;
            case 2:
                printf("Digite o valor para remover: ");
                scanf("%d", &valor);
                remover(arvore, valor);
                break;
            case 3:
                printf("Digite o valor para buscar: ");
                scanf("%d", &valor);
                res = buscar(arvore, arvore->raiz, valor);
                if (res != arvore->TNULL)
                    printf("Valor %d ENCONTRADO (Cor: %s).\n", valor, (res->cor == VERMELHO ? "Vermelho" : "Preto"));
                else
                    printf("Valor %d NAO encontrado.\n", valor);
                break;
            case 4:
                printf("Pre-Ordem: ");
                preOrdem(arvore, arvore->raiz);
                break;
            case 5:
                printf("Em-Ordem: ");
                emOrdem(arvore, arvore->raiz);
                break;
            case 6:
                printf("Pos-Ordem: ");
                posOrdem(arvore, arvore->raiz);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

int main() {
    ArvoreRB *arvore = criarArvore();
    menu(arvore);
    
    return 0;
}
