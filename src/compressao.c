#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "compressao.h"
#include "frequencia.h"
#include "huffman.h"
#include "bitio.h"


/* cabecalho: assinatura + 256 frequencias de 4 bytes (little-endian) */
static int escrever_cabecalho(FILE *saida, const unsigned int frequencias[256]) {
    const char assinatura[4] = {'H', 'U', 'F', '1'};
    int i;

    if (fwrite(assinatura, 1, 4, saida) != 4) {
        return -1;
    }

    for (i = 0; i < 256; i++) {
        uint32_t frequencia = (uint32_t) frequencias[i];
        unsigned char bytes[4];

        bytes[0] = frequencia & 0xFF;
        bytes[1] = (frequencia >> 8) & 0xFF;
        bytes[2] = (frequencia >> 16) & 0xFF;
        bytes[3] = (frequencia >> 24) & 0xFF;

        if (fwrite(bytes, 1, 4, saida) != 4) {
            return -1;
        }
    }

    return 0;
}


/* escreve o cabecalho e depois le o original de novo trocando cada byte
 * pelo seu codigo */
static int escrever_arquivo(FILE *entrada,
                            FILE *saida,
                            const unsigned int frequencias[256],
                            char codigos[256][256]) {
    BitWriter bw;
    int byte;

    if (escrever_cabecalho(saida, frequencias) != 0) {
        printf("Erro ao escrever cabecalho.\n");
        return -1;
    }

    bitwriter_init(&bw, saida);

    while ((byte = fgetc(entrada)) != EOF) {
        if (bitwriter_write_bits(&bw, codigos[byte]) != 0) {
            printf("Erro ao escrever bits.\n");
            return -1;
        }
    }

    if (ferror(entrada)) {
        printf("Erro ao ler arquivo original.\n");
        return -1;
    }

    if (bitwriter_flush(&bw) != 0) {
        printf("Erro ao finalizar arquivo comprimido.\n");
        return -1;
    }

    return 0;
}


int comprimir(const char *arquivo_entrada, const char *arquivo_saida) {
    unsigned int frequencias[256];
    unsigned long long tamanho_original = 0;
    char codigos[256][256] = {{0}};
    No *raiz;
    FILE *entrada;
    FILE *saida;
    int resultado = -1;
    int i;

    /* primeira leitura: conta os bytes */
    if (contar_frequencia(arquivo_entrada, frequencias) != 0) {
        printf("Erro ao ler o arquivo de entrada "
               "(nao abriu, falha de leitura ou arquivo com 4 GiB ou mais).\n");
        return -1;
    }

    for (i = 0; i < 256; i++) {
        tamanho_original += frequencias[i];
    }

    /* arquivo vazio nao tem arvore, entao so constroi se tiver dados */
    if (tamanho_original > 0) {
        raiz = construir_arvore(frequencias);

        if (raiz == NULL) {
            printf("Erro: memoria insuficiente para construir a arvore.\n");
            return -1;
        }

        gerar_codigos(raiz, codigos);

        /* depois dos codigos prontos a arvore nao serve mais */
        liberar_arvore(raiz);
    }

    entrada = fopen(arquivo_entrada, "rb");

    if (entrada == NULL) {
        printf("Erro ao abrir arquivo de entrada.\n");
        return -1;
    }

    saida = fopen(arquivo_saida, "wb");

    if (saida == NULL) {
        printf("Erro ao criar arquivo comprimido.\n");
    } else {
        resultado = escrever_arquivo(entrada, saida, frequencias, codigos);

        /* erro no fclose pode ser dado que nao foi gravado */
        if (fclose(saida) != 0) {
            printf("Erro ao fechar arquivo comprimido.\n");
            resultado = -1;
        }
    }

    fclose(entrada);

    return resultado;
}