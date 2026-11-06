#include <stdio.h>
#include "bitio.h"

void bitwriter_init(BitWriter *bw, FILE *arquivo) {
    bw->arquivo = arquivo;
    bw->buffer = 0;
    bw->nbits = 0;
}

int bitwriter_write_bit(BitWriter *bw, int bit) {
    bw->buffer <<= 1;

    if (bit != 0) {
        bw->buffer |= 1;
    }

    bw->nbits++;

    /* buffer cheio: manda o byte pro arquivo */
    if (bw->nbits == 8) {
        if (fputc(bw->buffer, bw->arquivo) == EOF) {
            return -1;
        }

        bw->buffer = 0;
        bw->nbits = 0;
    }

    return 0;
}

/* codigo e uma string de '0' e '1' */
int bitwriter_write_bits(BitWriter *bw, const char *codigo) {
    while (*codigo != '\0') {
        if (*codigo != '0' && *codigo != '1') {
            return -1;
        }

        if (bitwriter_write_bit(bw, *codigo - '0') != 0) {
            return -1;
        }

        codigo++;
    }

    return 0;
}

/* completa o ultimo byte com zeros e grava */
int bitwriter_flush(BitWriter *bw) {
    if (bw->nbits > 0) {
        bw->buffer <<= (8 - bw->nbits);

        if (fputc(bw->buffer, bw->arquivo) == EOF) {
            return -1;
        }

        bw->buffer = 0;
        bw->nbits = 0;
    }

    return fflush(bw->arquivo) == 0 ? 0 : -1;
}

void bitreader_init(BitReader *br, FILE *arquivo) {
    br->arquivo = arquivo;
    br->buffer = 0;
    br->nbits = 0;
}

/* retorna 0, 1 ou -1 se acabou o arquivo */
int bitreader_read_bit(BitReader *br) {

    if (br->nbits == 0) {
        int byte = fgetc(br->arquivo);

        if (byte == EOF) {
            return -1;
        }

        br->buffer = (uint8_t) byte;
        br->nbits = 8;
    }

    /* pega o bit mais da esquerda e empurra o resto */
    int bit = (br->buffer >> 7) & 1;

    br->buffer <<= 1;
    br->nbits--;

    return bit;
}

int bitreader_read_bits(BitReader *br, char *destino, size_t quantidade) {

    for (size_t i = 0; i < quantidade; i++) {
        int bit = bitreader_read_bit(br);

        if (bit == -1) {
            destino[i] = '\0';
            return -1;
        }

        destino[i] = bit ? '1' : '0';
    }

    destino[quantidade] = '\0';

    return 0;
}