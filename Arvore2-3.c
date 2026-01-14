/*
====================================================================
    UNIVERSIDADE FEDERAL DO AMAPÁ – UNIFAP
    Curso: Bacharelado em Ciência da Computação
    Disciplina: Programação II

    TRABALHO PRÁTICO – ÁRVORE 2-3

    Objetivo:
        Implementação de uma Árvore 2-3 com operações de:
        - Inserção de valores
        - Busca de valores
        - Remoção de nós
        - Percursos (em ordem e por nível)

    Integrantes do grupo:
        1 - Diego Maciel
        2 - Márleson Ferreira
        3 - Lucas
====================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ============================================================
// 1. ESTRUTURAS DE DADOS
// ============================================================

typedef struct No {
    int chaves[3];             // Máximo de 2 chaves (usamos 3 para auxiliar no overflow)
    struct No *filhos[4];      // Máximo de 3 filhos (usamos 4 para auxiliar no overflow)
    int num_chaves;            // Quantidade atual de chaves
    bool eh_folha;             // Verdadeiro se for folha
} No;

// Resultado de uma (split) para subir a chave mediana
typedef struct ResultadoSplit {
    int chave;
    No *filho_direito;
    bool ativo; // Indica se houve split
} ResultadoSplit;

// Estrutura para Fila (Usada na impressão por nível)
typedef struct NoFila {
    No *no_arvore;
    struct NoFila *prox;
} NoFila;

typedef struct {
    NoFila *frente;
    NoFila *tras;
} Fila;

// ============================================================
// 2. PROTÓTIPOS E UTILITÁRIOS
// ============================================================

No* criarNo(bool eh_folha);
void imprimirPorNivel(No *raiz);
void imprimirEmOrdem(No *raiz);
No* buscar(No *raiz, int chave);
void inserir(No **raiz, int chave);
void remover(No **raiz, int chave);

// Utilitários de Fila
void enfileirar(Fila *f, No *n);
No* desenfileirar(Fila *f);
bool filaVazia(Fila *f);

// ============================================================
// 3. OPERAÇÕES DE CRIAÇÃO E BUSCA
// ============================================================

No* criarNo(bool eh_folha) {
    No *novoNo = (No*)malloc(sizeof(No));
    novoNo->num_chaves = 0;
    novoNo->eh_folha = eh_folha;
    for (int i = 0; i < 4; i++) novoNo->filhos[i] = NULL;
    return novoNo;
}

No* buscar(No *raiz, int chave) {
    if (!raiz) return NULL;

    int i = 0;
    // Encontra a primeira chave maior ou igual a 'chave'
    while (i < raiz->num_chaves && chave > raiz->chaves[i]) {
        i++;
    }

    // Se encontrou a chave
    if (i < raiz->num_chaves && chave == raiz->chaves[i]) {
        return raiz;
    }

    // Se não encontrou e é folha, a chave não existe
    if (raiz->eh_folha) return NULL;

    // Desce para o filho apropriado
    return buscar(raiz->filhos[i], chave);
}

// ============================================================
// 4. LÓGICA DE INSERÇÃO (COM SPLIT/CISÃO)
// ============================================================

// Insere ordenado num array temporário do nó (quando há espaço)
void inserirEmNoNaoCheio(No *no, int chave, No *filho) {
    int i = no->num_chaves - 1;
    // Move chaves maiores para a direita
    while (i >= 0 && no->chaves[i] > chave) {
        no->chaves[i + 1] = no->chaves[i];
        no->filhos[i + 2] = no->filhos[i + 1];
        i--;
    }
    no->chaves[i + 1] = chave;
    no->filhos[i + 2] = filho;
    no->num_chaves++;
}

// Função recursiva interna de inserção
ResultadoSplit inserirRecursivo(No *no, int chave) {
    ResultadoSplit res = {0, NULL, false};

    // 1. Verifica se chave já existe (ignora duplicatas)
    for (int i = 0; i < no->num_chaves; i++) {
        if (no->chaves[i] == chave) return res;
    }

    // 2. Se é folha, tenta inserir
    if (no->eh_folha) {
        if (no->num_chaves < 2) {
            inserirEmNoNaoCheio(no, chave, NULL);
        } else {
            // Overflow na folha -> Realizar Split
            int chavesTemp[3];
            int i, j;
            
            // Ordena as 3 chaves (2 atuais + 1 nova)
            for(i=0, j=0; i < 2; i++, j++) {
                if(no->chaves[i] > chave && j == i) {
                    chavesTemp[j] = chave;
                    j++;
                }
                chavesTemp[j] = no->chaves[i];
            }
            if(j == 2) chavesTemp[2] = chave;

            // Cria novo nó irmão à direita
            No *novoDireita = criarNo(true);
            
            // Redistribui: [k0, k1, k2] -> k0 fica, k1 sobe, k2 vai p/ direita
            no->chaves[0] = chavesTemp[0];
            no->num_chaves = 1;
            
            novoDireita->chaves[0] = chavesTemp[2];
            novoDireita->num_chaves = 1;

            res.chave = chavesTemp[1]; // Chave mediana sobe
            res.filho_direito = novoDireita;
            res.ativo = true;
        }
        return res;
    }

    // 3. Se não é folha, desce recursivamente
    int i = 0;
    while (i < no->num_chaves && chave > no->chaves[i]) i++;
    
    ResultadoSplit resFilho = inserirRecursivo(no->filhos[i], chave);

    // 4. Trata o retorno do filho (se houve split lá embaixo)
    if (resFilho.ativo) {
        if (no->num_chaves < 2) {
            inserirEmNoNaoCheio(no, resFilho.chave, resFilho.filho_direito);
            res.ativo = false; // Split resolvido neste nível
        } else {
            // Overflow neste nó interno -> Split interno
            int chavesTemp[3];
            No *filhosTemp[4];
            
            // Reorganiza chaves com a que subiu
            int k_idx = 0; 
            int inserido = 0;
            for(int k=0; k < 2; k++) {
                if(!inserido && resFilho.chave < no->chaves[k]) {
                    chavesTemp[k_idx++] = resFilho.chave;
                    inserido = 1;
                }
                chavesTemp[k_idx++] = no->chaves[k];
            }
            if(!inserido) chavesTemp[k_idx++] = resFilho.chave;

            // Reorganiza filhos
            for(int k=0; k<3; k++) filhosTemp[k] = no->filhos[k];
            // Abre espaço para o novo filho vindo do split inferior
            for(int k=3; k > i+1; k--) filhosTemp[k] = filhosTemp[k-1];
            filhosTemp[i+1] = resFilho.filho_direito;

            // Cria novo nó interno irmão
            No *novoDireita = criarNo(false);
            
            // Divide chaves e filhos entre 'no' e 'novoDireita'
            no->chaves[0] = chavesTemp[0];
            no->filhos[0] = filhosTemp[0];
            no->filhos[1] = filhosTemp[1];
            no->num_chaves = 1;
            
            novoDireita->chaves[0] = chavesTemp[2];
            novoDireita->filhos[0] = filhosTemp[2];
            novoDireita->filhos[1] = filhosTemp[3];
            novoDireita->num_chaves = 1;

            res.chave = chavesTemp[1]; // Mediana sobe
            res.filho_direito = novoDireita;
            res.ativo = true;
        }
    }
    return res;
}

void inserir(No **raiz, int chave) {
    if (*raiz == NULL) {
        *raiz = criarNo(true);
        (*raiz)->chaves[0] = chave;
        (*raiz)->num_chaves = 1;
        return;
    }

    ResultadoSplit res = inserirRecursivo(*raiz, chave);
    if (res.ativo) {
        // A raiz dividiu, cria nova raiz (a árvore cresce em altura)
        No *novaRaiz = criarNo(false);
        novaRaiz->chaves[0] = res.chave;
        novaRaiz->filhos[0] = *raiz;
        novaRaiz->filhos[1] = res.filho_direito;
        novaRaiz->num_chaves = 1;
        *raiz = novaRaiz;
    }
}

// ============================================================
// 5. LÓGICA DE REMOÇÃO (FUSÃO E REDISTRIBUIÇÃO)
// ============================================================

void fundir(No *pai, int indice) {
    No *esquerda = pai->filhos[indice];
    No *direita = pai->filhos[indice+1];

    // Desce a chave do pai para o nó da esquerda
    esquerda->chaves[esquerda->num_chaves] = pai->chaves[indice];
    esquerda->num_chaves++;

    // Copia chaves e filhos do nó da direita para o da esquerda
    for (int i = 0; i < direita->num_chaves; ++i) {
        esquerda->chaves[esquerda->num_chaves] = direita->chaves[i];
        esquerda->filhos[esquerda->num_chaves] = direita->filhos[i];
        esquerda->num_chaves++;
    }
    esquerda->filhos[esquerda->num_chaves] = direita->filhos[direita->num_chaves];

    // Ajusta o pai preenchendo o buraco
    for (int i = indice; i < pai->num_chaves - 1; ++i) {
        pai->chaves[i] = pai->chaves[i+1];
        pai->filhos[i+1] = pai->filhos[i+2];
    }
    pai->num_chaves--;
    free(direita);
}

void pegarDoAnterior(No *pai, int indice) {
    No *filho = pai->filhos[indice];
    No *irmao = pai->filhos[indice-1];

    // Abre espaço no início do filho
    for (int i = filho->num_chaves-1; i >= 0; --i) 
        filho->chaves[i+1] = filho->chaves[i];
    
    if (!filho->eh_folha) {
        for (int i = filho->num_chaves; i >= 0; --i) 
            filho->filhos[i+1] = filho->filhos[i];
    }

    // Chave do pai desce para filho
    filho->chaves[0] = pai->chaves[indice-1];
    if (!filho->eh_folha) 
        filho->filhos[0] = irmao->filhos[irmao->num_chaves];

    // Maior chave do irmão sobe para o pai
    pai->chaves[indice-1] = irmao->chaves[irmao->num_chaves-1];

    filho->num_chaves++;
    irmao->num_chaves--;
}

void pegarDoProximo(No *pai, int indice) {
    No *filho = pai->filhos[indice];
    No *irmao = pai->filhos[indice+1];

    // Chave do pai desce para o final do filho
    filho->chaves[filho->num_chaves] = pai->chaves[indice];
    if (!filho->eh_folha) 
        filho->filhos[filho->num_chaves+1] = irmao->filhos[0];
    
    // Menor chave do irmão sobe para o pai
    pai->chaves[indice] = irmao->chaves[0];

    // Ajusta o irmão (remove o primeiro elemento)
    for (int i = 1; i < irmao->num_chaves; ++i) 
        irmao->chaves[i-1] = irmao->chaves[i];
    
    if (!irmao->eh_folha) {
        for (int i = 1; i <= irmao->num_chaves; ++i) 
            irmao->filhos[i-1] = irmao->filhos[i];
    }

    filho->num_chaves++;
    irmao->num_chaves--;
}

// Garante que o nó filho tenha chaves suficientes antes de descermos nele
void preencher(No *no, int indice) {
    // Tenta pegar do irmão anterior
    if (indice != 0 && no->filhos[indice-1]->num_chaves >= 2) {
        pegarDoAnterior(no, indice);
    }
    // Tenta pegar do irmão seguinte
    else if (indice != no->num_chaves && no->filhos[indice+1]->num_chaves >= 2) {
        pegarDoProximo(no, indice);
    }
    // Se não der empréstimo, faz a fusão (Merge)
    else {
        if (indice != no->num_chaves)
            fundir(no, indice);
        else
            fundir(no, indice-1);
    }
}

void removerDeNaoFolha(No *no, int indice) {
    int k = no->chaves[indice];

    // 1. Se filho à esquerda tem chaves suficientes, acha antecessor
    if (no->filhos[indice]->num_chaves >= 2) {
        No *atual = no->filhos[indice];
        while (!atual->eh_folha) atual = atual->filhos[atual->num_chaves];
        int antecessor = atual->chaves[atual->num_chaves-1];
        no->chaves[indice] = antecessor;
        remover(&no->filhos[indice], antecessor);
    }
    // 2. Se filho à direita tem chaves suficientes, acha sucessor
    else if (no->filhos[indice+1]->num_chaves >= 2) {
        No *atual = no->filhos[indice+1];
        while (!atual->eh_folha) atual = atual->filhos[0];
        int sucessor = atual->chaves[0];
        no->chaves[indice] = sucessor;
        remover(&no->filhos[indice+1], sucessor);
    }
    // 3. Ambos com poucas chaves -> Fundir
    else {
        fundir(no, indice);
        remover(&no->filhos[indice], k); // Remove do nó já fundido
    }
}

void removerRecursivo(No *no, int chave) {
    int idx = 0;
    while (idx < no->num_chaves && no->chaves[idx] < chave) idx++;

    // A chave está neste nó?
    if (idx < no->num_chaves && no->chaves[idx] == chave) {
        if (no->eh_folha) {
            // Remoção simples em folha
            for (int i = idx+1; i < no->num_chaves; ++i)
                no->chaves[i-1] = no->chaves[i];
            no->num_chaves--;
        } else {
            removerDeNaoFolha(no, idx);
        }
    } else {
        // A chave não está aqui
        if (no->eh_folha) {
            printf("Chave %d nao encontrada.\n", chave);
            return;
        }

        bool ultimo_filho = (idx == no->num_chaves);
        
        // Garante que o filho onde vamos descer tem chaves suficientes
        if (no->filhos[idx]->num_chaves < 2)
            preencher(no, idx);

        // Se houve fusão e era o último filho, o índice pode ter mudado
        if (ultimo_filho && idx > no->num_chaves)
            removerRecursivo(no->filhos[idx-1], chave);
        else
            removerRecursivo(no->filhos[idx], chave);
    }
}

void remover(No **raiz, int chave) {
    if (!*raiz) {
        printf("Arvore vazia.\n");
        return;
    }

    removerRecursivo(*raiz, chave);

    // Se a raiz ficou vazia após fusões
    if ((*raiz)->num_chaves == 0) {
        No *tmp = *raiz;
        if ((*raiz)->eh_folha)
            *raiz = NULL;
        else
            *raiz = (*raiz)->filhos[0];
        free(tmp);
    }
}

// ============================================================
// 6. EXIBIÇÃO
// ============================================================

void imprimirEmOrdem(No *raiz) {
    if (raiz != NULL) {
        for (int i = 0; i < raiz->num_chaves; i++) {
            imprimirEmOrdem(raiz->filhos[i]);
            printf("%d ", raiz->chaves[i]);
        }
        imprimirEmOrdem(raiz->filhos[raiz->num_chaves]);
    }
}

// Utilitários de Fila
void enfileirar(Fila *f, No *n) {
    NoFila *novoNo = (NoFila*)malloc(sizeof(NoFila));
    novoNo->no_arvore = n;
    novoNo->prox = NULL;
    if (f->tras) f->tras->prox = novoNo;
    f->tras = novoNo;
    if (!f->frente) f->frente = novoNo;
}

No* desenfileirar(Fila *f) {
    if (!f->frente) return NULL;
    NoFila *temp = f->frente;
    No *dado = temp->no_arvore;
    f->frente = f->frente->prox;
    if (!f->frente) f->tras = NULL;
    free(temp);
    return dado;
}

bool filaVazia(Fila *f) { return f->frente == NULL; }

void imprimirPorNivel(No *raiz) {
    if (!raiz) {
        printf("Arvore vazia.\n");
        return;
    }
    Fila f = {NULL, NULL};
    enfileirar(&f, raiz);
    enfileirar(&f, NULL); // Marcador de fim de nível

    while (!filaVazia(&f)) {
        No *atual = desenfileirar(&f);
        
        if (atual == NULL) { // Fim do nível
            printf("\n");
            if (!filaVazia(&f)) enfileirar(&f, NULL);
        } else {
            printf("[");
            for(int i=0; i < atual->num_chaves; i++) {
                printf("%d", atual->chaves[i]);
                if(i < atual->num_chaves-1) printf("|");
            }
            printf("] ");

            if (!atual->eh_folha) {
                for(int i=0; i <= atual->num_chaves; i++) {
                    if(atual->filhos[i]) enfileirar(&f, atual->filhos[i]);
                }
            }
        }
    }
}

// ============================================================
// 7. MENU PRINCIPAL
// ============================================================

int main() {
    No *raiz = NULL;
    int opcao, valor;

    do {
        printf("\n=== ARVORE 2-3 (Portugues) ===\n");
        printf("1. Inserir\n");
        printf("2. Remover\n");
        printf("3. Consultar\n");
        printf("4. Exibir Em Ordem\n");
        printf("5. Exibir por Niveis\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o valor para inserir: ");
                scanf("%d", &valor);
                inserir(&raiz, valor);
                break;
            case 2:
                printf("Digite o valor para remover: ");
                scanf("%d", &valor);
                remover(&raiz, valor);
                break;
            case 3:
                printf("Digite o valor para buscar: ");
                scanf("%d", &valor);
                if (buscar(raiz, valor)) printf("Valor %d encontrado.\n", valor);
                else printf("Valor %d NAO encontrado.\n", valor);
                break;
            case 4:
                printf("Em Ordem: ");
                imprimirEmOrdem(raiz);
                printf("\n");
                break;
            case 5:
                printf("Estrutura por Nivel:\n");
                imprimirPorNivel(raiz);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
