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


/* Inicializa a escrita de bits */
void bitwriter_init(BitWriter *bw, FILE *arquivo);

/* Escreve um bit no arquivo */
int bitwriter_write_bit(BitWriter *bw, int bit);

/* Escreve vários bits de um código de Huffman */
int bitwriter_write_bits(BitWriter *bw,
                         const char *codigo);

/* Finaliza a escrita, preenchendo o último byte */
int bitwriter_flush(BitWriter *bw);


/* Inicializa a leitura de bits */
void bitreader_init(BitReader *br, FILE *arquivo);

/* Lê um bit do arquivo. Retorna -1 no fim do arquivo */
int bitreader_read_bit(BitReader *br);

/* Lê vários bits e os armazena em uma string.
 * destino precisa ter espaço para quantidade + 1 caracteres. */
int bitreader_read_bits(BitReader *br,
                        char *destino,
                        size_t quantidade);

#endif