#ifndef FREQUENCIA_H
#define FREQUENCIA_H

/* Conta quantas vezes cada byte aparece no arquivo.
 * Retorna 0 se deu certo e -1 se deu erro (nao abriu, erro de leitura
 * ou arquivo com 4 GiB ou mais). Se der erro, nao use o vetor. */
int contar_frequencia(const char *nome_arquivo, unsigned int frequencias[256]);

#endif