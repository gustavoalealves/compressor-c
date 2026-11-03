
#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef struct No {
    unsigned char byte;
    unsigned int frequencia;

    struct No *esquerda;
    struct No *direita;
} No;

No *construir_arvore(const unsigned int frequencias[256]);

void gerar_codigos(const No *raiz, char codigos[256][256]);

void liberar_arvore(No *raiz);

#endif
