#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include "descompressao.h"
#include "bitio.h"
#include "huffman.h"

/* Compara duas assinaturas de 4 bytes (substitui memcmp) */
static int assinatura_valida(const unsigned char *lida,
                             const unsigned char *esperada) {
    int i;

    for (i = 0; i < 4; i++) {
        if (lida[i] != esperada[i]) {
            return 0;
        }
    }

    return 1;
}

static int eh_folha(const No *no) {
    return no != NULL && no->esquerda == NULL && no->direita == NULL;
}

/*
 * Le a assinatura e as 256 frequencias do cabecalho.
 * O tamanho original e a soma das frequencias.
 * Retorna 0 em sucesso e -1 se o cabecalho for invalido.
 */
static int ler_cabecalho(FILE *entrada,
                         unsigned int frequencias[256],
                         uint64_t *tamanho_original) {
    static const unsigned char assinatura_esperada[4] = {'H', 'U', 'F', '1'};
    unsigned char assinatura[4];
    int i;

    *tamanho_original = 0;

    if (fread(assinatura, 1, sizeof(assinatura), entrada) != sizeof(assinatura) ||
        !assinatura_valida(assinatura, assinatura_esperada)) {
        return -1;
    }

    for (i = 0; i < 256; i++) {
        unsigned char bytes[4];
        uint32_t frequencia;

        if (fread(bytes, 1, sizeof(bytes), entrada) != sizeof(bytes)) {
            return -1;
        }

        frequencia = (uint32_t)bytes[0]
            | ((uint32_t)bytes[1] << 8)
            | ((uint32_t)bytes[2] << 16)
            | ((uint32_t)bytes[3] << 24);

        frequencias[i] = (unsigned int)frequencia;
        *tamanho_original += frequencia;
    }

    /*
     * A soma das frequencias e a frequencia da raiz da arvore, que e
     * um unsigned int. Cabecalho com soma maior esta corrompido.
     */
    if (*tamanho_original > UINT_MAX) {
        return -1;
    }

    return 0;
}

static int descomprimir_dados(FILE *entrada,
                              FILE *saida,
                              const No *raiz,
                              uint64_t tamanho_original) {
    BitReader br;
    const No *atual;
    uint64_t bytes_escritos = 0;
    uint64_t i;

    if (entrada == NULL || saida == NULL ||
        (raiz == NULL && tamanho_original != 0)) {
        return -1;
    }

    if (tamanho_original == 0) {
        return 0;
    }

    /* Um unico byte distinto: a arvore e so uma folha */
    if (eh_folha(raiz)) {
        for (i = 0; i < tamanho_original; i++) {
            if (fputc(raiz->byte, saida) == EOF) {
                return -1;
            }
        }
        return 0;
    }

    bitreader_init(&br, entrada);
    atual = raiz;

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
            if (fputc(atual->byte, saida) == EOF) {
                return -1;
            }
            bytes_escritos++;
            atual = raiz;
        }
    }

    return 0;
}

int descomprimir(const char *arquivo_entrada, const char *arquivo_saida) {
    unsigned int frequencias[256];
    uint64_t tamanho_original;
    No *raiz = NULL;
    FILE *entrada;
    FILE *saida;
    int resultado = -1;

    entrada = fopen(arquivo_entrada, "rb");
    if (entrada == NULL) {
        return -1;
    }

    if (ler_cabecalho(entrada, frequencias, &tamanho_original) == 0) {
        raiz = construir_arvore(frequencias);

        /* Raiz NULL so e aceitavel quando o original era vazio */
        if (raiz != NULL || tamanho_original == 0) {
            saida = fopen(arquivo_saida, "wb");

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

                /* Em caso de erro, nao deixa arquivo parcial no disco */
                if (resultado != 0) {
                    remove(arquivo_saida);
                }
            }
        }
    }

    fclose(entrada);
    liberar_arvore(raiz);

    return resultado;
}