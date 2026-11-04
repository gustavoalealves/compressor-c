#include <stdio.h>
#include "descompressao.h"
#include "bitio.h"

static int eh_folha(const No *no) {
    return no != NULL && no->esquerda == NULL && no->direita == NULL;
}

int descomprimir(FILE *entrada,
                 FILE *saida,
                 const No *raiz,
                 uint64_t tamanho_original) {

    if (entrada == NULL || saida == NULL || raiz == NULL){
        return -1;
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
