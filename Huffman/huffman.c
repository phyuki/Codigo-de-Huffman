/*Trabalho Arvore Rubro Negra
Disciplina: Estrutura de Dados II
Engenharia da Computacao
Alunos: Lazaro Jose, Pedro Henrique Goes
*/

#include "huffman.h"
#include "lista.c"

NArvore *novaArvore(byte chave, int frequencia, NArvore *esq, NArvore *dir){
    NArvore *n;

    if ( ( n = malloc(sizeof(*n)) ) == NULL ) return NULL;
    n->chave = chave;
    n->frequencia = frequencia;
    n->esq = esq;
    n->dir = dir;

    return n;
}

void frequencia(FILE *entrada, unsigned int *listaBytes){
    byte chave;

    while ((fread(&chave, 1, 1, entrada)) >= 1){    //Enquanto o retorno da funcao fread for maior ou igual a 1
        listaBytes[(byte)chave]++;  //Incrementa o valor da frequencia
    }

    rewind(entrada);
}

lista *geraLista(unsigned int *listaBytes){
    lista *l = (lista *)malloc(sizeof(lista));    //Lista com a cabeca apontando para null e elementos valendo 0
    if(l != NULL){
        l->cabeca = NULL;
        l->elementos = 0;
    }
    int i;
    for (i = 0; i < 256; i++){
        if (listaBytes[i]){ //Se existe o byte
            NArvore *nodeArv = novaArvore(i, listaBytes[i], NULL, NULL);
            NLista *nodeL = novaLista(nodeArv);
            insereLista(nodeL, l);   //Insere na lista o no referente ao byte, com sua frequencia
        }
    }
    return l;
}

NArvore *geraArvore(lista *l){

    while (l->elementos > 1){               //Enquanto o número de elementos da lista for maior que 1
        NArvore *noEsq = retiraLista(l);    //Retira o no de menor frequencia da fila
        NArvore *noDir = retiraLista(l);    //retira outro no de menor frequencia da fila

        NArvore *soma = novaArvore('#', noEsq->frequencia + noDir->frequencia, noEsq, noDir);   //Preenche o caractere com # e a frequencia com a soma das frequências dos nos direito e esquerdo
        NLista *auxL = novaLista(soma);
        insereLista(auxL, l);               //Reinsere o nó soma na lista
    }

    return retiraLista(l);
}

int searchArvore(NArvore *n, byte c, char *buffer, int tam){
    if (!(n->esq || n->dir) && n->chave == c){
        buffer[tam] = '\0';
        return 1;
    }
    else{
        int status = 0;

        if (n->esq){
            buffer[tam] = '0';
            status = searchArvore(n->esq, c, buffer, tam + 1);
        }

        if (status == 0 && n->dir){
            buffer[tam] = '1';
            status = searchArvore(n->dir, c, buffer, tam + 1);
        }

        if (status == 0) buffer[tam] = '\0';

        return status;
    }
}

int percorreArvoreBit(FILE *saida, NArvore *raiz, NArvore *aux, unsigned char *buffer, int *ini, int tam){
    int j = (*ini);
    while(aux != NULL && j <= tam){                   //Percorre a árvore de Huffman usando os dados binarios de cada caractere do arquivo comprimido
        if(aux->esq == NULL && aux->dir == NULL){
            fwrite(&aux->chave, 1, 1, saida);         //Achando um nó sem filhos, temos o caractere para ser impresso no arquivo de saída
            aux = raiz;
            (*ini) = j;                               //O ponteiro inicial vai para onde está o indice j
            j--;
        }
        else{                                         //Caso nao ache o nó filho, continua percorrendo a árvore até achar o nó folha ou esgotar o buffer
            if(j == tam) break;
            else if(buffer[j] == '0') aux = aux->esq;
            else aux = aux->dir;
        }
        j++;
    }
    if(j == tam && aux == raiz) return j;               //Se o indice for igual a tamanho do buffer, significa que o buffer esgotou e foi inserido no arquivo todo seu conteudo
    else return j+1;                                    //Caso contrário, retorna tamanho + 1 para indicar a necessidade de mais itens para o buffer
}

void FreeArvore(NArvore *n){
    if(!n) return;
    else{
        NArvore *esq = n->esq;
        NArvore *dir = n->dir;
        free(n);
        FreeArvore(esq);
        FreeArvore(dir);
    }
}

void printList(NArvore *nodeA, unsigned int *listaBytes){
    lista *l = geraLista(listaBytes);
    NLista *nodeL = l->cabeca;

    char buffer[1024] = {0};
    if(nodeL == NULL) return;
    printf("\n| Tabela de frequencia e de conversao |\n");
    printf("------------------------------------------------\n");
    printf("Char \t- \tFreq \t- \tBits\n");
    while(nodeL){
        if(nodeL->no->frequencia > 0){
            searchArvore(nodeA, nodeL->no->chave, buffer, 0);
            printf("%c  \t- \t%d \t- \t%s\n", nodeL->no->chave, nodeL->no->frequencia, buffer);

        }
        nodeL = nodeL->prox;
    }
    printf("------------------------------------------------\n\n");
}

void comprimirArquivo(const char *arqEntrada, const char *arqSaida){
    unsigned int listaBytes[256] = {0};

    FILE *entrada = fopen(arqEntrada, "rb");    //Abre o arquivo entrada no modo de leitura

    if(!entrada){
        printf("\nErro ao ler o arquivo!");
        exit(0);
    }

    FILE *saida = fopen(arqSaida, "wb");    //Abre o arquivo saida no modo de escrita
    if(!saida){
        printf("\nErro ao ler o arquivo!");
        exit(0);
    }

    frequencia(entrada, listaBytes);
    lista *l = geraLista(listaBytes);
    NArvore *raiz = geraArvore(l);
    printList(raiz, listaBytes);

    fwrite(listaBytes, 256, sizeof(listaBytes[0]), saida);  //Grava a lista de frequência nos primeiros 256 bytes do arquivo
    fseek(saida, sizeof(unsigned int), SEEK_CUR);   //Move o ponteiro da saida para a posição posterior ao tamanho de um unsigned int

    byte c;
    unsigned int tamanho = 0;
    byte aux = 0;

    while (fread(&c, 1, 1, entrada) >= 1){  //Enquanto a quantidade de bytes lidos for maior ou igual a 1

        char buffer[1024] = {0};
        searchArvore(raiz, c, buffer, 0);   //Realiza a busca utilizando o byte c
        int i;
        for (i = 0; buffer[i]!= '\0'; i++){ //For que percorre o buffer
            if (buffer[i] == '1') aux = aux | (1 << ((7 - tamanho) % 8));
            tamanho++;

            if (tamanho % 8 == 0) { //Se ja formou um byte
                fwrite(&aux, 1, 1, saida);  //Escreve no arquivo
                aux = 0;
            }
        }
    }

    unsigned int sobra = 0;
    if(tamanho %8 != 0){
        fwrite(&aux, 1, 1, saida);  //Escreve no arquivo o que sobrou
        sobra = 8 - tamanho % 8;
    }
    fseek(saida, 256*sizeof(unsigned int), SEEK_SET); //Move o ponteiro da saida para 256 vezes o tamanho de um unsigned int
    fwrite(&sobra, sizeof(unsigned int), 1, saida);   //Salva o valor do tamanho no arquivo saida

    // Calcula tamanho dos arquivos
    fseek(entrada, 0L, SEEK_END);
    double tamanhoEntrada = ftell(entrada);

    fseek(saida, 0L, SEEK_END);
    double tamanhoSaida = ftell(saida);

    printf("Compressao concluida com sucesso!\n");
    printf("\nArquivo de entrada: %s (%.1f KB)\nArquivo de saida: %s (%.1f KB)\n",
           arqEntrada, tamanhoEntrada / 1024, arqSaida, tamanhoSaida / 1024);
    printf("Taxa de compressao: %.2f%%\n\n", (100 * tamanhoSaida) / tamanhoEntrada);

    FreeArvore(raiz);
    fclose(entrada);
    fclose(saida);
}

void descomprimirArquivo(const char *arqEntrada, const char *arqSaida){
    unsigned int listaBytes[256] = {0};

    FILE *entrada = fopen(arqEntrada, "rb");    //Abre o arquivo entrada no modo de leitura
    if(!entrada){
        printf("\nErro ao ler o arquivo!");
        exit(0);
    }

    FILE *saida = fopen(arqSaida, "wb");    //Abre o arquivo saida no modo de escrita
    if(!saida){
        printf("\nErro ao ler o arquivo!");
        exit(0);
    }

    //Leitura do cabeçalho do arquivo de entrada, contendo a lista de bytes com as frequências de Huffman
    fread(listaBytes, sizeof(listaBytes[0]), 256, entrada);
    lista *l = geraLista(listaBytes);
    NArvore *raiz = geraArvore(l);
    //No cabeçalho, também temos a sobra de bits dentro do ultimo byte inserido durante a compressao
    unsigned int sobra;
    fread(&sobra, sizeof(unsigned int), 1, entrada);

    //Variáveis auxiliares
    byte aux = 1;
    int pos = 0, tam = 0, *ini;
    ini = malloc(sizeof(int));
    (*ini) = 0;
    byte leitura, auxLeitura;
    unsigned char buffer[1024] = {0};
    NArvore *auxArvore = raiz;

    //Leitura de cada caractere do arquivo, mantendo sempre a preocupacao com o ultimo byte do arquivo que pode conter sobras
    fread(&leitura, 1, 1, entrada);
    while(feof(entrada) == 0){
        int auxSobra = sobra;
        fread(&auxLeitura, 1, 1, entrada);
        if(feof(entrada) == 0) auxSobra = 0;
        int i;

        //Adicao dos bits referentes ao byte lido no arquivo comprido dentro de um buffer
        for(i = 8; i > auxSobra; i--, pos++){
            byte aux2 = aux << i-1;
            aux2 = aux2 | leitura;
            if(aux2 != leitura) buffer[pos] = '0';
            else buffer[pos] = '1';
        }
        tam = tam + 8-auxSobra;

        //Uso da arvore de Huffman para a escrita no arquivo de saida com o buffer de bits dado acima
        int qtdChar = percorreArvoreBit(saida, raiz, auxArvore, buffer, ini, tam);
        if(qtdChar != tam)             //Caso o buffer tenha sido insuficiente
            pos = qtdChar-1;
        else{                          //Se o buffer foi usado por inteiro sem nenhuma sobra, realiza-se um reset em seu conteudo
            unsigned char aux[1024] = {0};
            strcpy(buffer, aux);
            pos = 0;                   //Os ponteiros de posicao e inicio do buffer tambem sao resetados
            tam = 0;
            (*ini) = 0;
        }

        leitura = auxLeitura;
    }

    printf("\nDescompressao concluida com sucesso!\n\n");

    FreeArvore(raiz);

    fclose(entrada);
    fclose(saida);
}
