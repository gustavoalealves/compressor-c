#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "compressao.h"
#include "frequencia.h"
#include "huffman.h"
#include "bitio.h"


/* Escreve o cabecalho do arquivo comprimido */
static int escrever_cabecalho(FILE *saida, const unsigned int frequencias[256]) {
    /* Assinatura que identifica o formato Huffman */
    const char assinatura[4] = {'H', 'U', 'F', '1'};
    int i;

    if (fwrite(assinatura, 1, 4, saida) != 4) {
        return -1;
    }

    /* Escreve as frequencias dos 256 possiveis bytes */
    for (i = 0; i < 256; i++) {
        uint32_t frequencia = (uint32_t) frequencias[i];
        unsigned char bytes[4];

        /* Converte a frequencia para little-endian */
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


/* Grava o cabecalho e os dados codificados (segunda leitura do original) */
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

    /* fgetc tambem devolve EOF em erro de leitura */
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


/* Funcao principal de compressao */
int comprimir(const char *arquivo_entrada, const char *arquivo_saida) {
    unsigned int frequencias[256];
    unsigned long long tamanho_original = 0;
    char codigos[256][256] = {{0}};
    No *raiz;
    FILE *entrada;
    FILE *saida;
    int resultado = -1;
    int i;

    /* Primeira leitura: frequencia de cada byte */
    if (contar_frequencia(arquivo_entrada, frequencias) != 0) {
        printf("Erro ao ler o arquivo de entrada "
               "(nao abriu, falha de leitura ou arquivo com 4 GiB ou mais).\n");
        return -1;
    }

    for (i = 0; i < 256; i++) {
        tamanho_original += frequencias[i];
    }

    /*
     * Arquivo vazio nao tem arvore (raiz NULL e normal).
     * Se ha dados e a raiz e NULL, faltou memoria.
     */
    if (tamanho_original > 0) {
        raiz = construir_arvore(frequencias);

        if (raiz == NULL) {
            printf("Erro: memoria insuficiente para construir a arvore.\n");
            return -1;
        }

        gerar_codigos(raiz, codigos);

        /* A arvore nao e mais necessaria: so os codigos sao usados */
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

        /* Um erro no fclose pode significar dados nao gravados */
        if (fclose(saida) != 0) {
            printf("Erro ao fechar arquivo comprimido.\n");
            resultado = -1;
        }
    }

    fclose(entrada);

    return resultado;
}