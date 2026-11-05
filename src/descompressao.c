#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include "descompressao.h"
#include "bitio.h"
#include "huffman.h"

static int eh_folha(const No *no) {
    return no != NULL && no->esquerda == NULL && no->direita == NULL;
}

static int descomprimir_dados(FILE *entrada,
                              FILE *saida,
                              const No *raiz,
                              uint64_t tamanho_original) {

    if (entrada == NULL || saida == NULL ||
        (raiz == NULL && tamanho_original != 0)) {
        return -1;
    }

    if (tamanho_original == 0) {
        return 0;
    }

    BitReader br;

    bitreader_init(&br, entrada);
    
    if (eh_folha(raiz)) {
        for (uint64_t i = 0; i < tamanho_original; i++){
        
            if(fputc(raiz->byte, saida) == EOF){
                return -1;
            }
        }
        return 0;
    }

    const No *atual = raiz;
    uint64_t bytes_escritos = 0;

    while (bytes_escritos < tamanho_original) {
        int bit = bitreader_read_bit(&br);

        if (bit == -1) {
            return -1;
        }

        if (bit == 0) {
            atual = atual->esquerda;
        } else {
            atual = atual->direita;
        }

        if (eh_folha(atual)) {
            if (fputc(atual->byte, saida) == EOF){
                return -1;
            }
            bytes_escritos++;
            atual = raiz;
        }
    }
    return 0;
}

int descomprimir(const char *arquivo_entrada, const char *arquivo_saida) {
    static const unsigned char assinatura_esperada[4] = {'H', 'U', 'F', '1'};
    unsigned int frequencias[256];
    uint64_t tamanho_original = 0;
    No *raiz = NULL;
    int resultado = -1;

    FILE *entrada = fopen(arquivo_entrada, "rb");
    if (entrada == NULL) {
        return -1;
    }

    unsigned char assinatura[4];
    if (fread(assinatura, 1, sizeof(assinatura), entrada) != sizeof(assinatura) ||
        memcmp(assinatura, assinatura_esperada, sizeof(assinatura)) != 0) {
        fclose(entrada);
        return -1;
    }

    for (int i = 0; i < 256; i++) {
        unsigned char bytes[4];
        if (fread(bytes, 1, sizeof(bytes), entrada) != sizeof(bytes)) {
            fclose(entrada);
            return -1;
        }

        uint32_t frequencia = (uint32_t)bytes[0]
            | ((uint32_t)bytes[1] << 8)
            | ((uint32_t)bytes[2] << 16)
            | ((uint32_t)bytes[3] << 24);

        if (frequencia > UINT_MAX) {
            fclose(entrada);
            return -1;
        }

        frequencias[i] = (unsigned int)frequencia;
        tamanho_original += frequencia;
    }

    raiz = construir_arvore(frequencias);
    if (raiz == NULL && tamanho_original != 0) {
        fclose(entrada);
        return -1;
    }

    FILE *saida = fopen(arquivo_saida, "wb");
    if (saida != NULL) {
        resultado = descomprimir_dados(
            entrada,
            saida,
            raiz,
            tamanho_original
        );
        if (fclose(saida) != 0) {
            resultado = -1;
        }
    }

    if (fclose(entrada) != 0) {
        resultado = -1;
    }
    liberar_arvore(raiz);

    return resultado;
}
