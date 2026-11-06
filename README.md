Prensa Binária - Sistema de compressão e descompressão de arquivos utilizando o algoritmo de Huffman.
1. Resumo
   
A Prensa Binária é um programa desenvolvido em linguagem C que realiza a compressão e a descompressão de arquivos por meio do algoritmo de Huffman.
O objetivo do projeto é reduzir o espaço ocupado pelos arquivos utilizando uma técnica de compressão sem perdas (lossless compression). Sendo assim, após a descompressão, o arquivo recuperado deve ser idêntico ao original.
O projeto foi desenvolvido como parte da disciplina MCTA028-15 – Programação Estruturada, da Universidade Federal do ABC (UFABC).

2. Funcionalidades: 
O programa oferece as seguintes funcionalidades:
Compressão de arquivos: realiza a análise da frequência dos bytes, constrói uma árvore de Huffman e gera um arquivo compactado com a extensão .pbz.
Descompressão de arquivos: recupera o conteúdo original a partir de um arquivo .pbz.
Preservação do arquivo original: os arquivos originais não são modificados durante o processo.
Informações sobre a compressão: apresenta o tamanho original, o tamanho compactado, a taxa de compressão e o tempo de execução.

3. Estrutura do projeto: 
O código-fonte está organizado nos seguintes arquivos:
main.c: interface principal com as opções do programa;
frequência.c: análise dos bytes e sua frequência no arquivo;
huffman.c: construção e manipulação da árvore de Huffman;
bitio.c: leitura e escrita dos bits nos arquivos;
compressão.c: Implementação do processo de compressão utilizando dos códigos binários variáveis criados pela árvore de huffman;
descompressão.c: Implementação do processo de descompressão.

4. Tecnologias utilizadas: 
Linguagem: C.
Compilador: Visual Studo Code.
Algoritmo de compressão: Huffman.
Manipulação de arquivos: biblioteca padrão de C.
Estruturas de dados: árvores binárias, estruturas (struct) e alocação dinâmica de memória.

5. Requisitos para execução: 
Para compilar e executar o projeto, é necessário possuir:
Um terminal ou ambiente de desenvolvimento compatível.
O arquivo executável ‘compressor.exe’ instalado.

6. Compilação e Execução: 

6.1 Compressão: 
Ao instalar o executável ‘compressor.exe’, pressione duas vezes sobre o arquivo para que o terminal seja aberto;
Digite o número corresponde a operação que deseja realizar, neste caso da compressão, o 1, então aperte a tecla Enter;
 Insira o caminho onde o arquivo está localizado, sem utilizar de aspas e aperte a tecla Enter;
Em seguida, digite o nome que o arquivo comprimido terá, caso não preencha com o tipo de extensão, a extensão .pbz será adicionada, e aperte a tecla Enter para dar continuidade;
O arquivo será comprimido e estará guardada na pasta Downloads da sua máquina.

6.2 Descompressão: 
Ao instalar o executável ‘compressor.exe’, pressione duas vezes sobre o arquivo para que o terminal seja aberto;
Digite o número corresponde a operação que deseja realizar, neste caso da descompressão, o 2, então aperte a tecla Enter;
Em seguida insira apenas o  nome do arquivo comprimido e aperta a tecla Enter para dar continuidade;
Digite o nome escolhido para o arquivo recuperado, e para obtê-lo na extensão do arquivo original para que seja possível lê-lo, digite também o tipo de extensão como .pdf ou .jpeg, e novamente Enter para que a operação seja realizada.
Este arquivo também estará na pasta Downloads da sua máquina.

 7. Formato dos arquivos: 
Os arquivos compactados pelo programa utilizam a extensão .pbz. O arquivo compactado contém um cabeçalho com uma assinatura de identificação do formato (HUF1) e as frequências de ocorrência dos 256 possíveis valores de byte. Essas informações permitem reconstruir a árvore de Huffman durante o processo de descompressão.

8. Limitações: 
A eficiência da compressão depende das características do arquivo de entrada. Arquivos com pouca repetição de dados ou que já utilizam algum método de compressão podem não apresentar redução de tamanho. Além disso, o cabeçalho utilizado pelo programa ocupa espaço no arquivo compactado, o que pode fazer com que arquivos pequenos aumentem de tamanho após a compressão.

9. Autores: 
Gustavo Alexandre Alves da Silva,  
Marinah Chidinma dos Santos Okeke,
Murilo Ticianelli Silva. 
Instituição: Universidade Federal do ABC (UFABC)
Disciplina: MCTA028-15 – Programação Estruturada
Ano: 2026
