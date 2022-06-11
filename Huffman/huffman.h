/*Trabalho Arvore Rubro Negra
Disciplina: Estrutura de Dados II
Engenharia da Computacao
Alunos: Lazaro Jose, Pedro Henrique Goes
*/

#ifndef _H_HUFFMAN
#define _H_HUFFMAN

//Declaracao das bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*Definicao do tipo de dados byte onde unsigned char
e o tipo que consegue gravar no intervalo de 0 a 255 bytes*/
typedef unsigned char byte;

/*Estrutura de um nó de uma arvore huffman, contendo uma int para a frequencia,
um byte para informar a chave e ponteiros pra um nó a esquerda e a direita*/
typedef struct Narvore{
    int frequencia;
    byte chave;
    struct Narvore *esq, *dir;
}NArvore;

/*Estrutura de um nó de uma lista, contendo uma ponteiro para um no de uma arvore,
e um ponteiro pra o proximo no da lista*/
typedef struct Nlista{
    NArvore *no;
    struct Nlista *prox;
} NLista;

/*Estrutura de um uma cabeca de lista (fila de prioridade), contendo um ponteiro
para um no de lista, e um inteiro com a quantidade de elementos*/
typedef struct lista{
    NLista *cabeca;
    int elementos;
} lista;

//------------------------Funcoes da fila de prioridade------------------------

/*Funcao que cria um novo no para a fila de prioridade*/
NLista *novaLista(NArvore *Narv);

/*Funcao que insere um novo no na fila de prioridade*/
void insereLista(NLista *n, lista *l);

/*Funcao que retira o no cabeca da lista*/
NArvore *retiraLista(lista *l);

//------------------------Funcoes da arvore de huffman------------------------

/*Funcao que cria um novo no para a arvore*/
NArvore *novaArvore(byte chave, int frequencia, NArvore *esq, NArvore *dir);

/*Funcao que conta a frequencia dos bytes no arquivo lido*/
void frequencia(FILE *entrada, unsigned int *listaBytes);

/*Funçao que constroi uma lista encadeada ordenada com os valores de frequencia de cada caractere*/
lista *geraLista(unsigned int *listaBytes);

/*Funcao que constroi a arvore de huffman*/
NArvore *geraArvore(lista *l);

/*Funcao que realiza a busca na arvore de huffman*/
int searchArvore(NArvore *n, byte c, char *buffer, int tam);

/*Metodo para percorrer uma árvore de Huffman, utilizando os bits que foram extraidos de um arquivo e inseridos num buffer
e caso encontre um nó folha, insere seu caractere no arquivo de saida passado como parâmetro*/
int percorreArvoreBit(FILE *saida, NArvore *raiz, NArvore *aux, unsigned char *buffer, int *ini, int tam);

/*Funcao que libera memoria da arvore*/
void FreeArvore(NArvore *n);

/*Funcao para a impressao de uma tabela com as frequencias de cada caracter encontrado no arquivo de entrada*/
void printList(NArvore *nodeA, unsigned int *listaBytes);

/*Funcao que comprime um arquivo utilizando o metodo de compressao de huffman*/
void comprimirArquivo(const char *arqEntrada, const char *arqSaida);

/*Funcao que descomprime um arquivo utilizando o metodo de compressao de huffman*/
void descomprimirArquivo(const char *arqEntrada, const char *arqSaida);

#endif
