#include <stdio.h>
#include <math.h>
#define p printf
#define s scanf
#include <string.h>
#include <stdlib.h>

//O codigo trabalha com lista encadeada

typedef struct _controle{
    int data[11];
    char nome[100];
    char posto[15];
    char cpf[13];
    char destino[20];
    char hora_entrada[5];
    char hora_saida[5];
    //ponteiros auxiliares para listagem e linkagem dos dados
    struct _controle *prox;
    struct _controle *ant;
} controle;


controle *ptr_inicio = NULL;

void 
