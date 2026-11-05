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
