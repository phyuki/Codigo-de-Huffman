/*Trabalho Arvore Rubro Negra
Disciplina: Estrutura de Dados II
Engenharia da Computacao
Alunos: Lazaro Jose, Pedro Henrique Goes
*/

#include "huffman.c"

int main(){
    int resp = 0;
    char entrada[256], saida[256];

    while(resp != 3){
        system("cls");
        printf("----Compressor e descompressor de arquivos----");
        printf("\n[1] Comprimir arquivo");
        printf("\n[2] Descomprimir arquivo");
        printf("\n[3] Sair");
        printf("\n>>> ");
        scanf("%d", &resp);

        switch (resp){
        case 1:
            system("cls");
            printf("Digite o nome do arquivo a ser comprimido: ");
            scanf("%s", entrada);
            printf("Digite o nome do arquivo de saida: ");
            scanf("%s", saida);
            comprimirArquivo(entrada,saida);
            system("pause");
            break;
        case 2:
            system("cls");
            printf("Digite o nome do arquivo a ser descomprimido: ");
            scanf("%s",entrada);
            printf("Digite o nome do arquivo de saida: ");
            scanf("%s",saida);
            descomprimirArquivo(entrada,saida);
            system("pause");
            break;
        case 3:
            printf("\nSaida Realizada!\n\n");
            break;
        default:
            printf("\nOpcao invalida!\n\n");
            break;
        }
    }
    return 0;
}
