#ifndef descompressao_h
#define descompressao_h

#include <stdio.h>
#include <stdint.h>
#include "huffman.h"

int descomprimir(FILE *entrada,
                 FILE *saida,
                 const No *raiz,
                 uint64_t tamanho_original);
#endif
