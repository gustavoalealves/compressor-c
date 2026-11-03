
#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

// Cria um novo nó na memória
static No *criar_no(unsigned char byte, unsigned int frequencia) {

    No *novo = malloc(sizeof(No));

    if (novo == NULL) {
        return NULL;
    }

    novo->byte = byte;
    novo->frequencia = frequencia;
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

// Libera recursivamente a memória da árvore
void liberar_arvore(No *raiz) {

    if (raiz == NULL) {
        return;
    }

    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);

    free(raiz);
}

// Constrói a árvore de Huffman
No *construir_arvore(const unsigned int frequencias[256]) {

    No *nos[511];
    int quantidade = 0;

    // Cria um nó para cada byte que aparece no arquivo
    for (int i = 0; i < 256; i++) {

        if (frequencias[i] > 0) {

            nos[quantidade] = criar_no(
                (unsigned char)i,
                frequencias[i]
            );

            if (nos[quantidade] == NULL) {

                for (int j = 0; j < quantidade; j++) {
                    liberar_arvore(nos[j]);
                }

                return NULL;
            }

            quantidade++;
        }
    }

    // Combina os dois nós de menor frequência
    while (quantidade > 1) {

        int menor1 = -1;
        int menor2 = -1;

        // Procura os dois menores
        for (int i = 0; i < quantidade; i++) {

            if (menor1 == -1 ||
                nos[i]->frequencia < nos[menor1]->frequencia) {

                menor2 = menor1;
                menor1 = i;

            } else if (menor2 == -1 ||
                       nos[i]->frequencia < nos[menor2]->frequencia) {

                menor2 = i;
            }
        }

        // Pega os dois nós encontrados
        No *esquerda = nos[menor1];
        No *direita = nos[menor2];

        // Cria o nó pai com a soma das frequências
        No *pai = criar_no(
            0,
            esquerda->frequencia + direita->frequencia
        );

        if (pai == NULL) {

            for (int i = 0; i < quantidade; i++) {
                liberar_arvore(nos[i]);
            }

            return NULL;
        }

        // Conecta os filhos ao pai
        pai->esquerda = esquerda;
        pai->direita = direita;

        // Descobre qual índice é maior
        int maior = (menor1 > menor2) ? menor1 : menor2;
        int menor = (menor1 < menor2) ? menor1 : menor2;

        // Remove os dois nós antigos do vetor
        nos[maior] = nos[quantidade - 1];
        quantidade--;

        nos[menor] = nos[quantidade - 1];
        quantidade--;

        // Adiciona o novo nó pai
        nos[quantidade] = pai;
        quantidade++;
    }

    if (quantidade == 0) {
        return NULL;
    }

    return nos[0];
}
