#include <stdlib.h>
#include "huffman.h"

/* strcpy feito na mao */
static void copiar_texto(char *destino, const char *origem) {

    int i = 0;

    while (origem[i] != '\0') {
        destino[i] = origem[i];
        i++;
    }

    destino[i] = '\0';
}

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

void liberar_arvore(No *raiz) {

    if (raiz == NULL) {
        return;
    }

    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);

    free(raiz);
}

No *construir_arvore(const unsigned int frequencias[256]) {

    No *nos[511];
    int quantidade = 0;

    /* um no folha pra cada byte que aparece */
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

    /* junta sempre os dois de menor frequencia ate sobrar um so */
    while (quantidade > 1) {

        int menor1 = -1;
        int menor2 = -1;

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

        No *esquerda = nos[menor1];
        No *direita = nos[menor2];

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

        pai->esquerda = esquerda;
        pai->direita = direita;

        /* remove os dois do vetor trocando pelo ultimo; o maior indice
         * tem que sair primeiro senao o outro muda de lugar */
        int maior = (menor1 > menor2) ? menor1 : menor2;
        int menor = (menor1 < menor2) ? menor1 : menor2;

        nos[maior] = nos[quantidade - 1];
        quantidade--;

        nos[menor] = nos[quantidade - 1];
        quantidade--;

        nos[quantidade] = pai;
        quantidade++;
    }

    if (quantidade == 0) {
        return NULL;
    }

    return nos[0];
}

static void percorrer_arvore(
    const No *raiz,
    char codigos[256][256],
    char caminho[256],
    int profundidade
) {

    if (raiz == NULL) {
        return;
    }

    if (raiz->esquerda == NULL && raiz->direita == NULL) {

        /* so tem um byte no arquivo: a raiz e a propria folha, usa "0" */
        if (profundidade == 0) {
            caminho[0] = '0';
            caminho[1] = '\0';
        } else {
            caminho[profundidade] = '\0';
        }

        copiar_texto(codigos[raiz->byte], caminho);

        return;
    }

    caminho[profundidade] = '0';
    percorrer_arvore(
        raiz->esquerda,
        codigos,
        caminho,
        profundidade + 1
    );

    caminho[profundidade] = '1';
    percorrer_arvore(
        raiz->direita,
        codigos,
        caminho,
        profundidade + 1
    );
}

void gerar_codigos(const No *raiz, char codigos[256][256]) {

    char caminho[256];

    for (int i = 0; i < 256; i++) {
        codigos[i][0] = '\0';
    }

    percorrer_arvore(raiz, codigos, caminho, 0);
}