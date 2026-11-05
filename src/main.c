#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "compressao.h"
#include "descompressao.h"

#define TAM_CAMINHO 512
#define EXTENSAO ".pbz"

/* ------------------------------------------------------------------ */
/* Funcoes auxiliares de texto (sem usar string.h)                     */
/* ------------------------------------------------------------------ */

static int tamanho_texto(const char *texto) {
    int n = 0;

    while (texto[n] != '\0') {
        n++;
    }

    return n;
}

static int textos_iguais(const char *a, const char *b) {
    int i = 0;

    while (a[i] != '\0' && a[i] == b[i]) {
        i++;
    }

    return a[i] == b[i];
}

static int termina_com(const char *texto, const char *sufixo) {
    int nt = tamanho_texto(texto);
    int ns = tamanho_texto(sufixo);
    int i;

    if (ns > nt) {
        return 0;
    }

    for (i = 0; i < ns; i++) {
        if (texto[nt - ns + i] != sufixo[i]) {
            return 0;
        }
    }

    return 1;
}

/* Acrescenta a extensao ao nome, se ele ainda nao a tiver.
 * Retorna 0 em caso de sucesso e -1 se nao houver espaco. */
static int garantir_extensao(char *nome, int capacidade) {
    int n;
    int i;

    if (termina_com(nome, EXTENSAO)) {
        return 0;
    }

    n = tamanho_texto(nome);

    if (n + tamanho_texto(EXTENSAO) + 1 > capacidade) {
        return -1;
    }

    for (i = 0; EXTENSAO[i] != '\0'; i++) {
        nome[n + i] = EXTENSAO[i];
    }

    nome[n + i] = '\0';

    return 0;
}

/* ------------------------------------------------------------------ */
/* Entrada de dados                                                    */
/* ------------------------------------------------------------------ */

/* Mostra a mensagem e le uma linha inteira (aceita espacos).
 * Retorna 0 se leu, -1 se a entrada terminou (EOF).
 * Se a linha for maior que o buffer, descarta o excesso e devolve
 * string vazia, que o chamador trata como invalida. */
static int ler_linha(const char *mensagem, char *destino, int tamanho) {
    int i;
    int c;
    int achou_fim = 0;

    printf("%s", mensagem);

    if (fgets(destino, tamanho, stdin) == NULL) {
        destino[0] = '\0';
        return -1;
    }

    for (i = 0; destino[i] != '\0'; i++) {
        if (destino[i] == '\n' || destino[i] == '\r') {
            destino[i] = '\0';
            achou_fim = 1;
            break;
        }
    }

    /* Ultima linha da entrada sem '\n' (ex.: entrada redirecionada)
     * e valida: nao e "linha longa demais". */
    if (!achou_fim && !feof(stdin)) {
        /* linha maior que o buffer: descarta o resto */
        while ((c = getchar()) != '\n' && c != EOF) {
            ;
        }
        destino[0] = '\0';
        printf("Caminho muito longo.\n");
    }

    return 0;
}

/* Le a opcao do menu. Retorna 1, 2 ou 3; 0 se invalida; -1 em EOF */
static int ler_opcao(void) {
    char linha[32];

    if (ler_linha("Escolha uma opção: ", linha, (int)sizeof(linha)) != 0) {
        return -1;
    }

    if (linha[0] >= '1' && linha[0] <= '3' && linha[1] == '\0') {
        return linha[0] - '0';
    }

    return 0;
}

/* ------------------------------------------------------------------ */
/* Arquivos                                                            */
/* ------------------------------------------------------------------ */

static int arquivo_existe(const char *caminho) {
    FILE *f = fopen(caminho, "rb");

    if (f == NULL) {
        return 0;
    }

    fclose(f);
    return 1;
}

/* Obtem o tamanho em bytes. Retorna 0 em sucesso e -1 em erro. */
static int tamanho_arquivo(const char *caminho, long *tamanho) {
    FILE *f = fopen(caminho, "rb");
    long fim;

    if (f == NULL) {
        return -1;
    }

    if (fseek(f, 0L, SEEK_END) != 0) {
        fclose(f);
        return -1;
    }

    fim = ftell(f);
    fclose(f);

    if (fim < 0) {
        return -1;
    }

    *tamanho = fim;
    return 0;
}

/* Valida entrada e saida. Retorna 1 se pode prosseguir, 0 se nao. */
static int validar_caminhos(const char *entrada, const char *saida) {
    char resposta[16];

    if (entrada[0] == '\0' || saida[0] == '\0') {
        printf("Nome de arquivo invalido.\n");
        return 0;
    }

    if (!arquivo_existe(entrada)) {
        printf("Arquivo de entrada nao encontrado: %s\n", entrada);
        return 0;
    }

    /* O original nunca pode ser sobrescrito */
    if (textos_iguais(entrada, saida)) {
        printf("O arquivo de saida deve ser diferente do de entrada.\n");
        return 0;
    }

    /* Se a saida ja existe, pede confirmacao */
    if (arquivo_existe(saida)) {
        printf("O arquivo %s ja existe.\n", saida);

        if (ler_linha("Deseja sobrescrever? (s/n): ", resposta,
                      (int)sizeof(resposta)) != 0) {
            return 0;
        }

        if (resposta[0] != 's' && resposta[0] != 'S') {
            printf("Operacao cancelada.\n");
            return 0;
        }
    }

    return 1;
}

/* ------------------------------------------------------------------ */
/* Operacoes                                                           */
/* ------------------------------------------------------------------ */

static void executar_compressao(void) {
    char entrada[TAM_CAMINHO];
    char saida[TAM_CAMINHO];
    long tam_original = 0;
    long tam_comprimido = 0;
    clock_t inicio;
    double segundos;
    int resultado;

    if (ler_linha("Digite o nome do arquivo a ser comprimido: ",
                  entrada, TAM_CAMINHO) != 0) {
        return;
    }

    if (ler_linha("Digite o nome do arquivo de saída (" EXTENSAO
                  " é acrescentada se faltar): ",
                  saida, TAM_CAMINHO) != 0) {
        return;
    }

    if (saida[0] != '\0' && garantir_extensao(saida, TAM_CAMINHO) != 0) {
        printf("Nome do arquivo de saida muito longo.\n");
        return;
    }

    if (!validar_caminhos(entrada, saida)) {
        return;
    }

    inicio = clock();
    resultado = comprimir(entrada, saida);
    segundos = (double)(clock() - inicio) / CLOCKS_PER_SEC;

    if (resultado != 0) {
        remove(saida); /* nao deixa arquivo incompleto */
        printf("Erro ao comprimir o arquivo.\n");
        return;
    }

    if (tamanho_arquivo(entrada, &tam_original) != 0 ||
        tamanho_arquivo(saida, &tam_comprimido) != 0) {
        printf("Arquivo comprimido, mas nao foi possivel medir os tamanhos.\n");
        return;
    }

    printf("\nArquivo comprimido com sucesso!\n");
    printf("Arquivo original: %s Tamanho: %ld Bytes\n", entrada, tam_original);
    printf("Arquivo comprimido: %s Tamanho: %ld Bytes\n", saida, tam_comprimido);

    if (tam_original > 0) {
        printf("Razão de compressão: %.2f\n",
               (double)tam_comprimido / (double)tam_original);
    } else {
        printf("Razão de compressão: indefinida (arquivo original vazio)\n");
    }

    printf("Tempo de compressão: %.3f s\n\n", segundos);
}

static void executar_descompressao(void) {
    char entrada[TAM_CAMINHO];
    char saida[TAM_CAMINHO];
    long tam_comprimido = 0;
    long tam_restaurado = 0;
    clock_t inicio;
    double segundos;
    int resultado;

    if (ler_linha("Digite o nome do arquivo a ser descomprimido: ",
                  entrada, TAM_CAMINHO) != 0) {
        return;
    }

    if (ler_linha("Digite o nome do arquivo restaurado: ",
                  saida, TAM_CAMINHO) != 0) {
        return;
    }

    if (!validar_caminhos(entrada, saida)) {
        return;
    }

    inicio = clock();
    resultado = descomprimir(entrada, saida);
    segundos = (double)(clock() - inicio) / CLOCKS_PER_SEC;

    if (resultado != 0) {
        printf("Erro ao descomprimir: arquivo invalido, corrompido ou "
               "nao gerado por este programa.\n");
        return;
    }

    if (tamanho_arquivo(entrada, &tam_comprimido) != 0 ||
        tamanho_arquivo(saida, &tam_restaurado) != 0) {
        printf("Arquivo descomprimido, mas nao foi possivel medir os tamanhos.\n");
        return;
    }

    printf("\nArquivo descomprimido com sucesso!\n");
    printf("Arquivo comprimido: %s Tamanho: %ld Bytes\n", entrada, tam_comprimido);
    printf("Arquivo restaurado: %s Tamanho: %ld Bytes\n", saida, tam_restaurado);
    printf("Tempo de descompressão: %.3f s\n\n", segundos);
}

int main(void) {
    int opcao;

    do {
        printf("Prensa Binária - UFABC\n");
        printf("1) Comprimir\n2) Descomprimir\n3) Sair\n");

        opcao = ler_opcao();

        switch (opcao) {
            case 1:
                executar_compressao();
                break;
            case 2:
                executar_descompressao();
                break;
            case 3:
                printf("Saindo...\n");
                break;
            case -1:
                /* fim da entrada (EOF): encerra sem loop infinito */
                printf("\nEntrada encerrada.\n");
                opcao = 3;
                break;
            default:
                printf("Opção inválida. Escolha 1, 2 ou 3.\n\n");
                break;
        }
    } while (opcao != 3);

    return 0;
}