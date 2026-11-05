
#include <stdio.h>
#include "bitio.h"

/* Inicializa a escrita de bits */
void bitwriter_init(BitWriter *bw, FILE *arquivo) {
    bw->arquivo = arquivo;
    bw->buffer = 0;
    bw->nbits = 0;
}

/* Escreve um bit individual */
int bitwriter_write_bit(BitWriter *bw, int bit) {
    bw->buffer <<= 1;

    if (bit != 0) {
        bw->buffer |= 1;
    }

    bw->nbits++;

    /*
     * Quando o buffer possui 8 bits,
     * escreve um byte no arquivo.
     */
    if (bw->nbits == 8) {
        if (fputc(bw->buffer, bw->arquivo) == EOF) {
            return -1;
        }

        bw->buffer = 0;
        bw->nbits = 0;
    }

    return 0;
}

/* Escreve uma sequência de bits */
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

/* Finaliza a escrita de bits */
int bitwriter_flush(BitWriter *bw) {
    if (bw->nbits > 0) {

        /*
         * Completa o último byte com zeros.
         */
        bw->buffer <<= (8 - bw->nbits);

        if (fputc(bw->buffer, bw->arquivo) == EOF) {
            return -1;
        }

        bw->buffer = 0;
        bw->nbits = 0;
    }

    return fflush(bw->arquivo) == 0 ? 0 : -1;
}

/* Inicializa a estrutura de leitura */
void bitreader_init(BitReader *br, FILE *arquivo) {
    br->arquivo = arquivo;
    br->buffer = 0;
    br->nbits = 0;
}

/* Lê um bit individual */
int bitreader_read_bit(BitReader *br) {

    /*
     * Se não há bits disponíveis,
     * lê o próximo byte do arquivo.
     */
    if (br->nbits == 0) {
        int byte = fgetc(br->arquivo);

        if (byte == EOF) {
            return -1;
        }

        br->buffer = (uint8_t) byte;
        br->nbits = 8;
    }

    /*
     * Extrai o bit mais significativo.
     */
    int bit = (br->buffer >> 7) & 1;

    /*
     * Desloca o buffer para o próximo bit.
     */
    br->buffer <<= 1;
    br->nbits--;

    return bit;
}

/* Lê uma sequência de bits */
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
