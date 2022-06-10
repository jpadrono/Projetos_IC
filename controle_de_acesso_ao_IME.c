#include <stdio.h>
#include <math.h>
#define p printf
#define s scanf
#include <string.h>
#include <stdlib.h>
/******************************************************************************
 *                              COMENTARIOS                                   *
 * 1.Achar um nome melhor para "TIPO"                                         *
 * 2.validacao do cpf l.120                                                   *
 * 3.confirmar os postos de oficiais                                          *
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
    //ponteiros auxiliares para listagem encadeada dos dados
    struct _controle *prox;
    struct _controle *ant;
} controle;


controle *ptr_inicio = NULL;

//funcoes e variaveis que serao utilizadas
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
    //obviamente n vai deixar assim, coloquemo nome, TAB, e simbolos que quiserem
    p("\t\tTELA BONITA DE CADASTRO\t\t\n");
    //menu que sera repetido ate a escolha da opcao N
    do{
        //para saber quando o do while eh repitido por escolha invalida
        //se valid != 0 o usuario escolheu uma opcao inexistente
        int valid = 0;
        // o ponteiro ptr_inicio eh a base da nossa lista encadeada
        if(!valid--) inserir(&ptr_inicio);
        p("\nDeseja cadastrar mais uma pessoa (S/N)?: ");
        s(" %c", &op);
        if(op != 's' && op != 'S' && op != 'n' && op != 'N'){
            p("Opcao invalida!\n");
            valid++;
            op = 's';
        }
    } while(op == 's' || op == 'S');

    listar(ptr_inicio);
    apagar(ptr_inicio);
    return 0;
}

void inserir(controle **ptr){
    // ponteiro de controle para ordenar a lista encadeada
    // as variaveis serao ordenadas no momento da criacao
    controle *n_ptr, *o_ptr, *ptr_aux;

    int ord;
    n_ptr = (controle*) malloc(sizeof(controle));
    if(n_ptr == NULL){
        p("Erro na alocação!\n");
        return;
    }
    ptr_aux = *ptr;
    o_ptr = NULL;

    //leitura dos valores do cadastrado
    //as validacoes ainda precisao ser feitas
    //validacao do cpf eh obrigatoria, mas eh interessante fazer das outras
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
                //como a ordenacao eh feita pelas data, criei uma variavel ord
                //como um inteiro, vacilitando a comparacao das variaveis
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
                //fazer um do while para loopar o scan do cpf, o processo eh
                //repetido enquanto o formato estiver errado
                p("Digite seu CPF no formato XXXXXXXXX-XX: ");
                s(" %[^\n]", var);
                //}while(validacaoc_cpf());
                strcpy(n_ptr->cpf, var);
                break;
        }
    }

    //caso seja o primeiro cadastro do arquivo
    if(ptr_aux == NULL){
        n_ptr->prox = NULL;
        *ptr = n_ptr;
    }
    else{
        //mesmo principio do insertion sort
        //desenhos podem ajudar a entender oq esta acontecendo
        while(ptr_aux != NULL && ptr_aux->ord < ord){
           o_ptr = ptr_aux;
           ptr_aux = ptr_aux->prox;
        }


        n_ptr->prox = ptr_aux;

        if(o_ptr == NULL){
            *ptr = n_ptr;
        } 
        else{
            o_ptr->prox = n_ptr;
        }
    }
    return; 
}
/****************************************************************************** 
*printa a linha com todas as informacoes do cadastro                          *
* para a apresentacao das litar por dia eh possivel a criacao de uma nova     *
* funcao imprimir_sem_ data onde a data n eh impressa                         *
* tbm eh possivel a chamada da funcao apresentar 2 variaveis, onde a variavel *
* 2 (int data) imprime a data caso seja TRUE(1) e n imprime caso seja FALSE(0)*
*******************************************************************************/
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

//ao mesmo tempo que lista toda a lista enumerada, tbm salva no arquivo
void listar(controle *ptr){
    //abre arquivo para escrever
    //ver com a professora ou com outros grupos se os dados anterioves devem
    //ficar salvos no arquivo "a" ou apagados a cada run "w"
    FILE *fptr;
    fptr = fopen("cadastro_de_acesso.txt", "a");

    
    //salva e lista todos do controle
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
    //reseta o ponteiro auxiiar para o inicio da lista
    ptr_aux = ptr_inicio;

    //printa apenas os ponteiros cujo tipo eh visitantes
    //processo se repete para as outros listas
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
//olhar aula VII parte 3
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
    //a funcao strstr diz se ptr eh substreng de postos
    if(strstr(postos, ptr)) return "of";
    return "erro";
}

// mesma estrutura do imprimir porem com a funcao de escrita em arquivo
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
