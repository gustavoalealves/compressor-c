# compressor-c
Implementação de um sistema de compressão e descompressão de arquivos utilizando a linguagem C e o algoritmo de Huffman.

Fluxo geral do compressor
1. Leitura do arquivo
   
   O programa abre o arquivo original e identifica seus bytes.
2. Análise de frequência
   
   Conta quantas vezes cada byte aparece no arquivo.
3. Construção da árvore de Huffman
   
   Utiliza as frequências para construir uma árvore que determinará os códigos binários.
4. Geração dos códigos
   
   Associa um código binário a cada byte.
5. Compressão
   
   Substitui os bytes originais pelos códigos e grava os bits no arquivo comprimido.
6. Descompressão
   
   Lê o arquivo comprimido e reconstrói os dados originais.

   compressor-c/
│
├── src/
│   ├── main.c
│   ├── frequencia.c
│   ├── huffman.c
│   ├── compressao.c
│   ├── descompressao.c
│   └── bitio.c
│
├── include/
│   ├── frequencia.h
│   ├── huffman.h
│   ├── compressao.h
│   ├── descompressao.h
│   └── bitio.h
│
├── tests/
│
├── Makefile
├── README.md
└── .gitignore

Agora vamos entender cada parte.
Arquivo	Responsabilidade
main.c ->	Apresentar o menu e controlar o fluxo do programa.
frequencia.c	-> Ler o arquivo e contar a frequência de cada byte.
frequencia.h ->	Declarar as funções e estruturas do módulo de frequência.
huffman.c ->	Construir a árvore de Huffman e gerar os códigos.
huffman.h ->	Declarar as estruturas e funções da árvore.
compressao.c ->	Coordenar o processo de compressão e gerar o arquivo comprimido.
compressao.h	-> Declarar as funções de compressão.
descompressao.c ->	Reconstruir o arquivo original a partir do comprimido.
descompressao.h ->	Declarar as funções de descompressão.
bitio.c	 -> Escrever e ler bits individuais em arquivos binários.
bitio.h	 -> Declarar as funções de manipulação de bits.

O Makefile será responsável por compilar os arquivos juntos. A pasta tests armazenará nossos arquivos e procedimentos de teste.

Como vocês estão construindo o projeto do zero, eu faria nessa ordem:
1. main.c: criar a estrutura principal e o menu.
2. frequencia.c e frequencia.h: implementar a leitura e a contagem dos bytes.
3. huffman.c e huffman.h: construir a árvore e gerar os códigos.
4. bitio.c e bitio.h: implementar a leitura e escrita de bits.
5. compressao.c e compressao.h: integrar os módulos para gerar o arquivo comprimido.
6. descompressao.c e descompressao.h: reconstruir o arquivo original.

