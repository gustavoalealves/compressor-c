#include <stdio.h>
#include "frequencia.h"

int contar_frequencia(const char *nome_arquivo, unsigned int frequencias[256]) {
    FILE *arquivo;
    unsigned int total = 0;
    int byte;
    int i;

    for (i = 0; i < 256; i++) {
        frequencias[i] = 0;
    }

    arquivo = fopen(nome_arquivo, "rb");
    if (arquivo == NULL) {
        return -1;
    }

    while ((byte = fgetc(arquivo)) != EOF) {
        frequencias[byte]++;
        total++;

        /* total voltou a zero: o arquivo tem 4 GiB ou mais */
        if (total == 0) {
            fclose(arquivo);
            return -1;
        }
    }

    /* fgetc tambem devolve EOF em erro de leitura */
    if (ferror(arquivo)) {
        fclose(arquivo);
        return -1;
    }

    fclose(arquivo);
    return 0;
}