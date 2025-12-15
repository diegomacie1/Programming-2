#include <stdio.h>
#include <stdlib.h>

// Definição das cores
enum Color { VERMELHO, PRETO };

// Estrutura do Nó
typedef struct Node {
    int data;
    enum Color color;
    struct Node *left, *right, *parent;
} Node;

// Estrutura da Árvore
typedef struct RBTree {
    Node *root;
    Node *TNULL; // Sentinela (substitui o NULL)
} RBTree;

// --- Protótipos das Funções ---
RBTree* criarArvore();
void inicializarTNULL(Node *node, Node *parent);
void leftRotate(RBTree *t, Node *x);
void rightRotate(RBTree *t, Node *x);
void inserir(RBTree *t, int key);
void inserirFixUp(RBTree *t, Node *k);
void remover(RBTree *t, int data);
void removerFixUp(RBTree *t, Node *x);
void transplant(RBTree *t, Node *u, Node *v);
Node* minimum(RBTree *t, Node *node);
Node* buscar(RBTree *t, Node *node, int data);
void preOrdem(RBTree *t, Node *node);
void emOrdem(RBTree *t, Node *node);
void posOrdem(RBTree *t, Node *node);
void menu(RBTree *t);

// --- Implementação ---

// Inicializa a árvore e o nó Sentinela
RBTree* criarArvore() {
    RBTree *t = (RBTree*)malloc(sizeof(RBTree));
    t->TNULL = (Node*)malloc(sizeof(Node));
    t->TNULL->color = PRETO;
    t->TNULL->left = NULL;
    t->TNULL->right = NULL;
    t->root = t->TNULL;
    return t;
}

// Rotação para a Esquerda
void leftRotate(RBTree *t, Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != t->TNULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        t->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// Rotação para a Direita
void rightRotate(RBTree *t, Node *x) {
    Node *y = x->left;
    x->left = y->right;
    if (y->right != t->TNULL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        t->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

// Correção após Inserção
void inserirFixUp(RBTree *t, Node *k) {
    Node *u;
    while (k->parent->color == VERMELHO) {
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left; // Tio
            if (u->color == VERMELHO) {
                // Caso 1: Tio é vermelho
                u->color = PRETO;
                k->parent->color = PRETO;
                k->parent->parent->color = VERMELHO;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    // Caso 2: Tio é preto e k é filho à esquerda (Triângulo)
                    k = k->parent;
                    rightRotate(t, k);
                }
                // Caso 3: Tio é preto e k é filho à direita (Linha)
                k->parent->color = PRETO;
                k->parent->parent->color = VERMELHO;
                leftRotate(t, k->parent->parent);
            }
        } else {
            u = k->parent->parent->right; // Tio
            if (u->color == VERMELHO) {
                // Caso 1 espelhado
                u->color = PRETO;
                k->parent->color = PRETO;
                k->parent->parent->color = VERMELHO;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    // Caso 2 espelhado
                    k = k->parent;
                    leftRotate(t, k);
                }
                // Caso 3 espelhado
                k->parent->color = PRETO;
                k->parent->parent->color = VERMELHO;
                rightRotate(t, k->parent->parent);
            }
        }
        if (k == t->root) break;
    }
    t->root->color = PRETO;
}

// Inserção Principal
void inserir(RBTree *t, int key) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->parent = NULL;
    node->data = key;
    node->left = t->TNULL;
    node->right = t->TNULL;
    node->color = VERMELHO; // Novos nós são sempre vermelhos

    Node *y = NULL;
    Node *x = t->root;

    while (x != t->TNULL) {
        y = x;
        if (node->data < x->data) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    node->parent = y;
    if (y == NULL) {
        t->root = node;
    } else if (node->data < y->data) {
        y->left = node;
    } else {
        y->right = node;
    }

    if (node->parent == NULL) {
        node->color = PRETO;
        return;
    }

    if (node->parent->parent == NULL) {
        return;
    }

    inserirFixUp(t, node);
}

// Auxiliar: Transplante de nós (usado na remoção)
void transplant(RBTree *t, Node *u, Node *v) {
    if (u->parent == NULL) {
        t->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

// Auxiliar: Encontrar mínimo
Node* minimum(RBTree *t, Node *node) {
    while (node->left != t->TNULL) {
        node = node->left;
    }
    return node;
}

// Correção após Remoção (O caso mais complexo da RB Tree)
void removerFixUp(RBTree *t, Node *x) {
    Node *s;
    while (x != t->root && x->color == PRETO) {
        if (x == x->parent->left) {
            s = x->parent->right;
            // Caso 1: Irmão é vermelho
            if (s->color == VERMELHO) {
                s->color = PRETO;
                x->parent->color = VERMELHO;
                leftRotate(t, x->parent);
                s = x->parent->right;
            }
            // Caso 2: Irmão é preto e ambos sobrinhos são pretos
            if (s->left->color == PRETO && s->right->color == PRETO) {
                s->color = VERMELHO;
                x = x->parent;
            } else {
                // Caso 3: Irmão preto, sobrinho esquerdo vermelho, direito preto
                if (s->right->color == PRETO) {
                    s->left->color = PRETO;
                    s->color = VERMELHO;
                    rightRotate(t, s);
                    s = x->parent->right;
                }
                // Caso 4: Irmão preto, sobrinho direito vermelho
                s->color = x->parent->color;
                x->parent->color = PRETO;
                s->right->color = PRETO;
                leftRotate(t, x->parent);
                x = t->root;
            }
        } else { // Simétrico (espelhado)
            s = x->parent->left;
            if (s->color == VERMELHO) {
                s->color = PRETO;
                x->parent->color = VERMELHO;
                rightRotate(t, x->parent);
                s = x->parent->left;
            }
            if (s->right->color == PRETO && s->left->color == PRETO) {
                s->color = VERMELHO;
                x = x->parent;
            } else {
                if (s->left->color == PRETO) {
                    s->right->color = PRETO;
                    s->color = VERMELHO;
                    leftRotate(t, s);
                    s = x->parent->left;
                }
                s->color = x->parent->color;
                x->parent->color = PRETO;
                s->left->color = PRETO;
                rightRotate(t, x->parent);
                x = t->root;
            }
        }
    }
    x->color = PRETO;
}

// Remoção Principal
void remover(RBTree *t, int data) {
    Node *z = t->TNULL;
    Node *x, *y;
    Node *temp = t->root;

    // Buscar o nó a ser removido
    while (temp != t->TNULL) {
        if (temp->data == data) {
            z = temp;
            break;
        }
        if (temp->data <= data) {
            temp = temp->right;
        } else {
            temp = temp->left;
        }
    }

    if (z == t->TNULL) {
        printf("Valor %d nao encontrado na arvore.\n", data);
        return;
    }

    y = z;
    int y_original_color = y->color;

    if (z->left == t->TNULL) {
        x = z->right;
        transplant(t, z, z->right);
    } else if (z->right == t->TNULL) {
        x = z->left;
        transplant(t, z, z->left);
    } else {
        y = minimum(t, z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    
    free(z); // Libera memória do nó removido

    if (y_original_color == PRETO) {
        removerFixUp(t, x);
    }
    printf("Valor %d removido com sucesso.\n", data);
}

// Busca Simples
Node* buscar(RBTree *t, Node *node, int data) {
    if (node == t->TNULL || data == node->data) {
        return node;
    }
    if (data < node->data) {
        return buscar(t, node->left, data);
    }
    return buscar(t, node->right, data);
}

// Impressão (V = Vermelho, P = Preto)
void preOrdem(RBTree *t, Node *node) {
    if (node != t->TNULL) {
        printf("%d(%c) ", node->data, (node->color == VERMELHO) ? 'V' : 'P');
        preOrdem(t, node->left);
        preOrdem(t, node->right);
    }
}

void emOrdem(RBTree *t, Node *node) {
    if (node != t->TNULL) {
        emOrdem(t, node->left);
        printf("%d(%c) ", node->data, (node->color == VERMELHO) ? 'V' : 'P');
        emOrdem(t, node->right);
    }
}

void posOrdem(RBTree *t, Node *node) {
    if (node != t->TNULL) {
        posOrdem(t, node->left);
        posOrdem(t, node->right);
        printf("%d(%c) ", node->data, (node->color == VERMELHO) ? 'V' : 'P');
    }
}

// Menu Interativo
void menu(RBTree *t) {
    int opcao, valor;
    Node* res;
    
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
                inserir(t, valor);
                break;
            case 2:
                printf("Digite o valor para remover: ");
                scanf("%d", &valor);
                remover(t, valor);
                break;
            case 3:
                printf("Digite o valor para buscar: ");
                scanf("%d", &valor);
                res = buscar(t, t->root, valor);
                if (res != t->TNULL)
                    printf("Valor %d ENCONTRADO (Cor: %s).\n", valor, (res->color == VERMELHO ? "Vermelho" : "Preto"));
                else
                    printf("Valor %d NAO encontrado.\n", valor);
                break;
            case 4:
                printf("Pre-Ordem: ");
                preOrdem(t, t->root);
                break;
            case 5:
                printf("Em-Ordem: ");
                emOrdem(t, t->root);
                break;
            case 6:
                printf("Pos-Ordem: ");
                posOrdem(t, t->root);
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
    RBTree *arvore = criarArvore();
    menu(arvore);
    
    // (Opcional) Implementar função para liberar memória total da árvore aqui
    return 0;
}