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

        /* se o total deu a volta e zerou, o arquivo tem 4 GiB ou mais */
        if (total == 0) {
            fclose(arquivo);
            return -1;
        }
    }

    /* o fgetc tambem retorna EOF quando da erro de leitura */
    if (ferror(arquivo)) {
        fclose(arquivo);
        return -1;
    }

    fclose(arquivo);
    return 0;
}