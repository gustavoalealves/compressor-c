
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "compressao.h"
#include "frequencia.h"
#include "huffman.h"
#include "bitio.h"


// Escreve o cabecalho do arquivo comprimido
static int escrever_cabecalho(FILE *saida, unsigned int frequencias[256]) {

    // Assinatura que identifica o formato Huffman
    const char assinatura[4] = {'H', 'U', 'F', '1'};

    if (fwrite(assinatura, 1, 4, saida) != 4) {
        return -1;
    }

    // Escreve as frequencias dos 256 possiveis bytes
    for (int i = 0; i < 256; i++) {

        uint32_t frequencia = (uint32_t) frequencias[i];

        // Converte a frequencia para little-endian
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


// Funcao principal de compressao
int comprimir(const char *arquivo_entrada, const char *arquivo_saida) {

    // Abrir o arquivo original
    FILE *entrada = fopen(arquivo_entrada, "rb");

    if (entrada == NULL) {
        printf("Erro ao abrir arquivo de entrada.\n");
        return -1;
    }

    // Calcular frequencias dos bytes
    unsigned int frequencias[256];

    contar_frequencia(arquivo_entrada, frequencias);

    // Construir arvore de Huffman
    No *raiz = construir_arvore(frequencias);

    // Verificar se o arquivo possui algum byte
    unsigned long long tamanho_original = 0;

    for (int i = 0; i < 256; i++) {
        tamanho_original += frequencias[i];
    }

    // Se houver dados, gerar os codigos de Huffman
    char codigos[256][256] = {{0}};

    if (raiz != NULL) {
        gerar_codigos(raiz, codigos);
    }

    // Criar arquivo de saida
    FILE *saida = fopen(arquivo_saida, "wb");

    if (saida == NULL) {
        printf("Erro ao criar arquivo comprimido.\n");

        fclose(entrada);
        liberar_arvore(raiz);

        return -1;
    }

    // Escrever cabecalho
    if (escrever_cabecalho(saida, frequencias) != 0) {
        printf("Erro ao escrever cabecalho.\n");

        fclose(entrada);
        fclose(saida);
        liberar_arvore(raiz);

        return -1;
    }

    // Inicializar escritor de bits
    BitWriter bw;
    bitwriter_init(&bw, saida);

    // Ler o arquivo original e escrever os codigos
    unsigned char byte;

    while (fread(&byte, sizeof(unsigned char), 1, entrada) == 1) {

        if (bitwriter_write_bits(&bw, codigos[byte]) != 0) {
            printf("Erro ao escrever bits.\n");

            fclose(entrada);
            fclose(saida);
            liberar_arvore(raiz);

            return -1;
        }
    }

    // Verificar erro de leitura
    if (ferror(entrada)) {
        printf("Erro ao ler arquivo original.\n");

        fclose(entrada);
        fclose(saida);
        liberar_arvore(raiz);

        return -1;
    }

    // Finalizar escrita dos bits
    if (bitwriter_flush(&bw) != 0) {
        printf("Erro ao finalizar arquivo comprimido.\n");

        fclose(entrada);
        fclose(saida);
        liberar_arvore(raiz);

        return -1;
    }

    // Fechar arquivos
    fclose(entrada);
    fclose(saida);

    // Liberar memoria da arvore
    liberar_arvore(raiz);

    return 0;
}
