#ifndef FREQUENCIA_H
#define FREQUENCIA_H

/*
 * Conta quantas vezes cada byte aparece no arquivo.
 * Retorna 0 em sucesso e -1 em caso de erro (nao conseguiu abrir,
 * erro de leitura ou arquivo com 4 GiB ou mais, que estouraria os
 * contadores de 32 bits). Em caso de erro, o conteudo de
 * frequencias nao deve ser usado.
 */
int contar_frequencia(const char *nome_arquivo, unsigned int frequencias[256]);

#endif