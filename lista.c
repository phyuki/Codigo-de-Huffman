/*Trabalho Arvore Rubro Negra
Disciplina: Estrutura de Dados II
Engenharia da Computacao
Alunos: Lazaro Jose, Pedro Henrique Goes
*/

#include "huffman.h"

NLista *novaLista(NArvore *Narv){
    NLista *n;

    if ( (n = malloc(sizeof(*n))) == NULL ) return NULL;
    n->no = Narv;
    n->prox = NULL;

    return n;
}

void insereLista(NLista *n, lista *l){
    if (!l->cabeca) l->cabeca = n;

    else if (n->no->frequencia < l->cabeca->no->frequencia){    //Se a frequencia do no for menor que a frequencia da cabeca
        n->prox = l->cabeca;                                      //o novo no vai ser a cabeca
        l->cabeca = n;
    }

    else{
        NLista *aux = l->cabeca->prox;
        NLista *aux2 = l->cabeca;

        while (aux && aux->no->frequencia <= n->no->frequencia){    //While que percorre a lista e insere em uma posicao de acordo com a frequencia
            aux2 = aux;
            aux = aux2->prox;
        }

        aux2->prox = n;
        n->prox = aux;
    }

    l->elementos++;
}

NArvore *retiraLista(lista *l){
    NLista *aux = l->cabeca;
    NArvore *aux2 = aux->no;

    l->cabeca = aux->prox;  //Aponta a cabeca para o segundo elemento da lista
    free(aux);
    aux = NULL;
    l->elementos--;
    return aux2;
}

