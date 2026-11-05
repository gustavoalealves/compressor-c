##include <stdio.h>
#include <stdlib.h>
#include "compressao.h"
#include "descompressao.h"

int main(void){ 
    int opcao = 0;
    char arquivo_entrada[256];
    char arquivo_saida[256];

do {
    printf("Compressor-c\n"); //pensei em prensa binária como nome pro compressor 
    printf("1 - Comprimir\n2 - Descomprimir\n3 - Sair\n");
    printf("Escolha uma opção:");
    scanf("%d", &opcao);

     if (opcao != 1 && opcao != 2 && opcao != 3){
        printf("Opção inválida. Escolha 1, 2 ou 3.\n");
    }
} while (opcao != 1 && opcao != 2 && opcao != 3);


   switch(opcao) {
        case 1: 
        
            printf("Digite o nome do arquivo a ser comprimido: ");
            scanf("%255s", arquivo_entrada);
            printf("Digite o nome do arquivo de saída: ");
            scanf("%255s", arquivo_saida);

            if (comprimir(arquivo_entrada, arquivo_saida) == 0) {
                printf("Arquivo comprimido com sucesso!\n");
            } else {
                printf("Erro ao comprimir o arquivo.\n");
            }
            break;
        case 2:
            printf("Digite o nome do arquivo a ser descomprimido: ");
            scanf("%255s", arquivo_entrada);
            printf("Digite o nome do arquivo de saída: ");
            scanf("%255s", arquivo_saida);

            if (descomprimir(arquivo_entrada, arquivo_saida) == 0) {
                printf("Arquivo descomprimido com sucesso!\n");
            } else {
                printf("Erro ao descomprimir o arquivo.\n");
            }
            break;
        case 3:
            printf("Saindo...\n");
            break;
        default:
        printf("Opção inválida. Escolha 1, 2 ou 3.\n");
        break;    
    }
return 0;
}
