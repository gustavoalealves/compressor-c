#include <stdio.h>
#include "frequencia.h"

void contar_frequencia(const char *nome_arquivo, unsigned int frequencias[256]) {
    for (int i = 0; i < 256; i++) {
        frequencias[i] = 0; 
    }

    FILE *arquivo = fopen(nome_arquivo, "rb"); 
    if (arquivo == NULL) return; 

    unsigned char byte;
    
    while (fread(&byte, sizeof(unsigned char), 1, arquivo) == 1) {
        frequencias[byte]++;
    }
    fclose(arquivo);
}
