#include <stdio.h>
#include <math.h>
#define p printf
#define s scanf
#include <string.h>
#include <stdlib.h>
/******************************************************************************
 *                              COMENTARIOS                                   *
 * 1.Achar um nome melhor para "TIPO"                                         *
 * ****************************************************************************/
//O codigo trabalha com lista encadeada
char postos[] = "Asp ofTenCapMajTCelGen brig Gen Div Gen Ex";

typedef struct _controle{
    int dia, mes, ano, ord;
    char nome[100];
    char tipo[20];
    char cpf[13];
    char destino[20];
    char hora_entrada[6];
    char hora_saida[6];
    //ponteiros auxiliares para listagem e linkagem dos dados
    struct _controle *prox;
    struct _controle *ant;
} controle;


controle *ptr_inicio = NULL;

//funcoes que serao utilizadas
void salvar(controle *ptr, FILE* aqr);
void inserir(controle **ptr);
void listar();
void apagar();
char *isoficial(char *ptr);
void imprimir(controle *ptr);
char op;
void sort_data();
int data_ord[100], ord, n=0;

int main(){
    p("\t\tTELA BONITA DE CADASTRO\t\t\n");
    do{
        inserir(&ptr_inicio);
        p("\nDeseja cadastrar mais uma pessoa (S/N)?: ");
        s(" %c", &op);
    } while(op == 's' || op == 'S');

    listar(ptr_inicio);
    apagar(ptr_inicio);
    return 0;
}

void inserir(controle **ptr){

    controle *n_ptr, *o_ptr, *ptr_aux;

    int ord;
    n_ptr = (controle*) malloc(sizeof(controle));
    if(n_ptr == NULL){
        p("Erro na alocação!\n");
        return;
    }
    ptr_aux = *ptr;
    o_ptr = NULL;

    char var[100];
    for(int i = 0; i < 7; i++){
        switch(i){
            case 0:
                p("Digite seu TIPO: ");
                s(" %[^\n]", var);
                strcpy(n_ptr->tipo, var);
                break;
            case 1:
                p("Digite seu Nome Completo: ");
                s(" %[^\n]", var);
                strcpy(n_ptr->nome, var);
                break;
            case 2:
                p("Digite a Data no formato dd/mm/aaaa: ");
                s(" %d/%d/%d", &n_ptr->dia, &n_ptr->mes, &n_ptr->ano);
                ord = n_ptr->dia + n_ptr->mes*100 + n_ptr->ano*10000;
                n_ptr->ord = ord;
                break;
            case 3:
                p("Digite a Hora de Entrada: ");
                s(" %[^\n]", var);
                strcpy(n_ptr->hora_entrada, var);
                break; 
            case 4:
                p("Digite a Hora de Saida: ");
                s(" %[^\n]", var);
                strcpy(n_ptr->hora_saida, var);
                break;
            case 5:
                p("Digite seu Destino: ");
                s(" %[^\n]", var);
                strcpy(n_ptr->destino, var);
                break;
            case 6:
                p("Digite seu CPF no formato XXXXXXXXX-XX: ");
                s(" %[^\n]", var);
                strcpy(n_ptr->cpf, var);
                break;
        }
    }

    if(ptr_aux == NULL){
        n_ptr->prox = NULL;
        *ptr = n_ptr;
    }
    else{
        while(ptr_aux != NULL && ptr_aux->ord < ord){
           o_ptr = ptr_aux;
           ptr_aux = ptr_aux->prox;
        }

        n_ptr->prox = ptr_aux;

        if(o_ptr ==NULL){
            *ptr = n_ptr;
        } 
        else{
            o_ptr->prox = n_ptr;
        }
    }
    return; 
}

void imprimir(controle *ptr){
    for(int i = 0; i < 7; i++){
        switch(i){
            case 0:
                p(" %s\t", ptr->tipo);
                break;
            case 1:
                p(" %s\t", ptr->nome);
                break;
            case 2:
                p(" %d/%d/%d\t", ptr->dia, ptr->mes, ptr->ano);
                break;
            case 3:
                p(" %s\t", ptr->hora_entrada);
                break;
            case 4:
                p(" %s\t", ptr->hora_saida);
                break;
            case 5:
                p(" %s\t", ptr->destino);
                break;
            case 6:
                p(" %s\t", ptr->cpf);
                break;
        }
    }
    putchar('\n');
}

void listar(controle *ptr){
    FILE *fptr;

    fptr = fopen("cadastro_de_acesso.txt", "a");

    

    controle *ptr_aux = ptr_inicio;
    if(ptr_aux == NULL){
        p("Lista Vazia!");
    }
    while(ptr_aux != NULL){
        salvar(ptr_aux, fptr);
        imprimir(ptr_aux);
        ptr_aux = ptr_aux->prox;
        p("\n");
    }

    ptr_aux = ptr_inicio;

    p("\nLista de visitantes\n");
    while(ptr_aux != NULL){
        if(!strcmp(ptr_aux->tipo, "Visitante")){
            imprimir(ptr_aux);
        }
        ptr_aux = ptr_aux->prox;
        p("\n");
    }

    ptr_aux = ptr_inicio;

    p("\nLista de of\n");
    while(ptr_aux!= NULL){
        if(!strcmp(isoficial(ptr_aux->tipo), "of")){
            imprimir(ptr_aux);
        }
        ptr_aux = ptr_aux->prox;
        p("\n");
    }
    ptr_aux = ptr_inicio;

    p("\nLista de grad\n");
    while(ptr_aux!= NULL){
        if(!strcmp(ptr_aux->tipo, "grad")){
            imprimir(ptr_aux);
        }
        ptr_aux = ptr_aux->prox;
        p("\n");
    }

    ptr_aux = ptr_inicio;

    p("\nLista de sc\n");
    while(ptr_aux!= NULL){
        if(!strcmp(ptr_aux->tipo, "SC")){
            imprimir(ptr_aux);
        }
        ptr_aux = ptr_aux->prox;
        p("\n");
    }

    fclose(fptr);
}

void apagar(controle *ptr_aux){
    if(ptr_aux != NULL){
        while(ptr_aux->prox != NULL){
            if(ptr_aux->ant != NULL) free(ptr_aux->ant);
            ptr_aux = ptr_aux->prox;
        }
        if(ptr_aux->ant != NULL) free(ptr_aux->ant);
        free(ptr_aux);
    }
    ptr_inicio = NULL;
}

char *isoficial(char *ptr){
    if(strstr(postos, ptr)) return "of";
    return "erro";
}

void salvar(controle *ptr, FILE *arq){
    char tab = '\t', el = '\n'; 
    for(int i = 0; i < 7; i++){
        switch(i){
            case 0:
                fputs(ptr->tipo, arq);
                fputc(tab, arq);
                break;
            case 1:
                fputs(ptr->nome, arq);
                fputc(tab, arq);
                break;
            case 2:
                fprintf(arq, " %d/%d/%d", ptr->dia, ptr->mes, ptr->ano);
                fputc(tab, arq);
                break;
            case 3:
                fputs(ptr->hora_entrada, arq);
                fputc(tab, arq);
                break;
            case 4:
                fputs(ptr->hora_saida, arq);
                fputc(tab, arq);
                break;
            case 5:
                fputs(ptr->destino, arq);
                fputc(tab, arq);
                break;
            case 6:
                fputs(ptr->cpf, arq);
                fputc(tab, arq);
                break;
        }
    }
    fputc(el, arq);
}
