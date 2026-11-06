#ifndef bitio_h
#define bitio_h

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

typedef struct{
    FILE *arquivo;
    uint8_t buffer;
    int nbits;
} BitWriter;

typedef struct {
    FILE *arquivo;
    uint8_t buffer;
    int nbits;
} BitReader;


void bitwriter_init(BitWriter *bw, FILE *arquivo);

int bitwriter_write_bit(BitWriter *bw, int bit);

/* codigo e uma string tipo "0110" */
int bitwriter_write_bits(BitWriter *bw,
                         const char *codigo);

/* tem que chamar no final pra gravar o ultimo byte */
int bitwriter_flush(BitWriter *bw);


void bitreader_init(BitReader *br, FILE *arquivo);

/* retorna -1 quando acaba o arquivo */
int bitreader_read_bit(BitReader *br);

/* destino precisa ter espaco para quantidade + 1 caracteres */
int bitreader_read_bits(BitReader *br,
                        char *destino,
                        size_t quantidade);

#endif