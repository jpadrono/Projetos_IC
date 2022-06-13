#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define NO_DE_CARACTERES 256

//lembrar de tirar os acentos e cecidilha
//verificar se todos os allocs foram livres
//verificar se todos os arquivos foram fechados

typedef struct _password{
    char finalidade[50];
    char senha[100];
} password;

password *ptr;

void salvar_senha(password *ptr);
void criar_senha();
void apresentar_senha();
void apagar_senha();
int senha_valida(char *ptr);
int count_line();
void lista_vazia();
void vanish();
void pause_ns(int n);
int n = 0, tamanho_da_senha = 16, fim = 0, sistema = -1;
char ch = '\n';
time_t t1, t2;

//strings para geracao da senha automatica e validacao da senha manual (sem cecidilha)
char alfabeto_min[] = {"zxcvbnmasdfghjklqwertyuiop"};
char alfabeto_ma[] = {"ZXCVBNMASDFGHJKLQWERTYUIOP"};
char numeros_e_especiais[] = {"0123456789@#$%&"};

// funcao que conta quais caracteres tem na string
int *caracteres_da_string(char *str){
    int *count = (int*) calloc(sizeof(int), NO_DE_CARACTERES);
    for(int i = 0; str[i]; i++){
        count[*(str+i)]++; 
    }
    return count;
}

int main(){
    do{
        printf("\t\t-------------------------------\n");
        printf("\t\t|ESCOLHA O SISTEMA OPERACIONAL|\n");
        printf("\t\t-------------------------------\n\n");
        printf("1 - Windows\n2 - Linux\n");
        scanf(" %d", &sistema);
        if(sistema != 1 && sistema != 2){
            printf("Opcao Invalida!");
            getchar();
        }
        vanish();
    }while(sistema != 1 && sistema != 2);

    FILE *fptr;
    fptr = open("senhas_salvas.bin", "rb");
    /*******************************************************************
    * seta a seed da funcao rand como o tempo desde 01/01/1970         *
    * assim para cada execucao teremos uma nova seed                   *
    * interessante pensar em formas mais seguras de gerar uma seed     *
    * pois sabendo o tempo exato em que o programa foi rodado          *
    * torna possivel recriar a senha gerada                            *
    *******************************************************************/
    srand(time(NULL));
    int op_menu = 0;
    //Primeir tela de selecao (quando n tiver nenhuma senha)
    // Tela de selecao com as escolhas
    // A tela ira aparecer ate que o usuario esoclha uma opcao existente
    while(1){
        do{
            lista_vazia(fptr);
            printf("\t\t----------------\n");
            printf("\t\t|MENU PRINCIPAL|\n");
            printf("\t\t----------------\n");
            printf("1 - Criar nova senha\n");
            printf("2 - Apresentar senha salva\n");
            printf("3 - Apagar senha salva\n");
            printf("4 - sair\n");
            scanf(" %d", &op_menu);
            
            if(op_menu == 4){
                fim++;
                break;
            }
            if(op_menu != 1 && op_menu != 2 && op_menu != 3){
                printf("opcao invalida\n");
                getchar();
            };
        } while(op_menu != 1 && op_menu != 2 && op_menu != 3);

        if(fim){
            for(int i = 0; i < n; i++){
                free(ptr[i].senha);
                free(ptr[i].finalidade);
            }
            return 0;
        }

        switch(op_menu){
            case 1:
                criar_senha();
                vanish();
                break;

            case 2:
                vanish();
                printf("\t\t----------------------\n");
                printf("\t\t|LISTA DE FINALIDADES|\n");
                printf("\t\t----------------------\n");
                apresentar_senha(); 
                system("pause");
                vanish();
                break;
                
            case 3:
                
                vanish();
                printf("\t\t----------------------\n");
                printf("\t\t|LISTA DE FINALIDADES|\n");
                printf("\t\t----------------------\n");
                apagar_senha(); 
                pause_ns(2);
                vanish();
                break;
        }
    }
}

void apresentar_senha(){
    FILE *fptr;
    fptr = fopen("senhas_salvas.bin", "rb");

    if(fptr == NULL){
        printf("Erro ao abrir o arquivo!/n");
        exit(0);
    }

    //como as finalidades estao nas linhas impares, quanto o contador de '\n'
    // for par, escreve a linha
    int count_end_line = 0;
    /**************************************************************************
     *  //O codigo esta contando um caracterer apos o ultimo '\n' gerando um  *
     *  //novo numero na lista de finalidades                                 *
     *                                                                        *
     * while(!feof(fptr)){                                                    *
     *   if((count_end_line+1)&1 && ch == '\n'){                              *
     *       printf("%d - ", (count_end_line+2)/2);                           *
     *   }                                                                    *
     *   ch = fgetc(fptr);                                                    *
     *   if(!(count_end_line&1)){                                             *
     *       printf("%c", ch);                                                *
     *   }                                                                    *
     *   if(ch == '\n') count_end_line++;                                     *
     *   }                                                                    *
     *                                                                        *
     * //como forma de contornar o proble acima, primeiro vamos contar a      *
     * //quantidade de '\n'                                                   *
     **************************************************************************/
    
    //conta o numero de linhas do arquivo
    while(!feof(fptr)){
        fread(&ch, sizeof(char), 1, fptr);
        if(ch == '\n') count_end_line++;
    }
    // reseta o ponteiro para o inicio do arquivo
    rewind(fptr);

    int n_line = 0;
    //reseta o ch
    ch = '\n';
    while(!feof(fptr)){
        if((n_line+1)&1 && ch == '\n' && n_line < count_end_line-1){                              
            printf("%d - ", (n_line+2)/2);
        }
        fread(&ch, sizeof(char), 1, fptr);
        if(!(n_line&1)){                                             
            printf("%c", ch);                                                
        }                                                                    
        if(ch == '\n') n_line++;
    }

    rewind(fptr);
    n_line = 0;
    //inicio do processo para localizacao e escrita da senha
    int escolha;
    do{
        printf("\nDigite o numero da senha a ser exibida: ");
        scanf(" %d", &escolha);
        if(escolha > count_line() || escolha < 1){
            printf("Escolha invalida!\n");
            getchar();
        }
    } while(escolha > count_line() || escolha < 1);

    printf("\nFinalidade: ");
    while(!feof(fptr)){

        fread(&ch, sizeof(char), 1, fptr);
        //printa a finalidade
        if(n_line == 2*escolha-2){
            printf("%c", ch);
            if(ch == '\n') printf("Senha: ");
        }
        //printa a senha
        if(n_line == 2*escolha-1){
            printf("%c", ch);
        }
        if(ch == '\n') n_line++;
    }
    
    fclose(fptr);


}

int count_line(){
    int count_n_line = 0;
    FILE *fptr;
    fptr = fopen("senhas_salvas.bin", "rb");
    while(!feof(fptr)){
        if(fread(&ch, sizeof(char), 1, fptr) != 1){
            /*
            printf("Erro na leitura do arquivo!\n");
            exit(0);
            */
        }
        if(ch == '\n') count_n_line++;  
    }

    fclose(fptr);
    return (count_n_line)/2;
}

void apagar_senha(){
    // inicio quase igual ao apresentar senha
    //adicao do ponteiro auxiliar para escrita do novo arquivo de senhas
    FILE *fptr, *fptr_aux;
    fptr = fopen("senhas_salvas.bin", "rb");


    if(fptr == NULL){
        printf("Erro ao abrir o arquivo!/n");
        exit(0);
    }
    int count_end_line = 0;
    
    while(!feof(fptr)){
        if(fread(&ch, sizeof(char), 1, fptr) != 1){
            /*
            printf("Erro na leitura do arquivo!\n");
            exit(0);
            */
        }
        if(ch == '\n') count_end_line++;   
    }
    rewind(fptr);

    int n_line = 0;

    ch = '\n';
    fptr_aux = fopen("senhas_aux.bin", "wb");
    if(fptr == NULL){
        printf("Erro ao abrir o arquivo!/n");
        exit(0);
    }
    while(!feof(fptr)){
        if((n_line+1)&1 && ch == '\n' && n_line < count_end_line-1){                              
            printf("%d - ", (n_line+2)/2);
        }
        fread(&ch, sizeof(char), 1, fptr);
        if(!(n_line&1)){                                             
            printf("%c", ch);                                                
        }                                                                    
        if(ch == '\n') n_line++;
    }

    rewind(fptr);
    n_line = 0;

    //inicio do processo de apagar a senha
    int escolha = 0;
    do{
        printf("\nDigite o numero da senha a ser excluida: ");
        scanf(" %d", &escolha);
        if(escolha > count_line() || escolha < 1){
            printf("Escolha invalida!\n");
            getchar();
        }
    } while(escolha > count_line() || escolha < 1);

    while(1){
        if(fread(&ch, sizeof(char), 1, fptr) != 1){
            printf("Erro na leitura do arquivo!\n");
            exit(0);
        }
        if(n_line != (escolha*2-2) && n_line != (escolha*2-1)){
            if(fwrite(&ch, sizeof(char), 1, fptr_aux) != 1){
                printf("Erro na escrita do arquivo!\n");
                exit(0);
            }
        }
        if(ch == '\n') n_line++;
        if(n_line == count_end_line-1) break;
    }

    //os arquivos serao abertos com as funcoes invertidas mantendo fptr como
    // o arquivo principal
    n_line = 0;
    fclose(fptr);
    fclose(fptr_aux);

    fptr = fopen("senhas_salvas.bin", "w+b");
    fptr_aux = fopen("senhas_aux.bin", "rb");

    while(!feof(fptr_aux)){
        if(ch == '\n') ++n_line;
        if(n_line == count_end_line-2) break;
        if(fread(&ch, sizeof(char), 1, fptr_aux) != 1){

            printf("Erro na leitura do arquivo!\n");
            exit(0);
        }
        if(fwrite(&ch, sizeof(char), 1, fptr) != 1){

            printf("Erro na leitura do arquivo!\n");
            exit(0);
            }
    }

    fclose(fptr);
    fclose(fptr_aux);
    printf("\n\t\t\tSENHA EXCLUIDA COM SUCESSO\n");
}

void criar_senha(){
    vanish();
    password *ptr = NULL;
    // Tela de selecao com a escolha da formacao da senha
    int op_senha;
    printf("\t\t---------------------------- \n");
    printf("\t\t| TELA DE CRIACAO DE SENHA |\n");
    printf("\t\t----------------------------\n\n");
    do{
        printf("Selecione o Metodo de Criacao:\n");
        printf("1 - Digitacao Manual\n");
        printf("2 - Geracao Automatica\n");
        scanf(" %d", &op_senha);
        if(op_senha != 1 && op_senha != 2){
            printf("Opcao invalida\n");
            getchar();
        }
    }while(op_senha != 1 && op_senha != 2);

    //aloca memoria para um novo no
    ptr = (password *) malloc(sizeof(password));

    *ptr[n].finalidade = NULL;
    printf("\nDigite a Finalidade da Senha: ");
    scanf(" %[^\n]", ptr[n].finalidade);
    vanish();

    switch(op_senha){
        case 1:
            printf("\n\tA SENHA DEVE TER NO MINIMO:\n");
            printf("8 Caracteres\n");
            printf("1 Letra Maiuscula e 1 Minuscula\n");
            printf("1 Caractere Numerico ou Especial(@, #, $, %%, ou &)\n");
            do{
                
                printf("\nDigite a Senha: ");
                scanf(" %[^\n]", ptr[n].senha);
            } while (!senha_valida(ptr[n].senha));
            salvar_senha(&ptr[n]);

            //para listas encadeadas
            ptr = (password *) realloc(ptr, ((n)+1)*sizeof(password));
            if(ptr == NULL){
                printf("Erro de alocacao");
                exit(1);
            }
        break; 
        case 2:
            *ptr[n].senha = NULL;
            char temp[17] = {"\0"};
            for(int i = 0; i < tamanho_da_senha; i++){
                /***************************************************************
                *rng com peso para decidir o tipo de caracter que sera inserido*
                *pesos:                                                        *
                *número ou caractere especial 2;                               *
                *letra maiúscula 7;                                            *
                *letra minúscula 7;                                            *
                ***************************************************************/
               int var = rand() % 16;
                if(var < 2){
                    temp[i] = numeros_e_especiais[rand() % strlen(numeros_e_especiais)];
                }
                else if(var < 9){
                    temp[i] = alfabeto_min[rand() % strlen(alfabeto_min)];
                }
                else{
                    temp[i] = alfabeto_ma[rand() % strlen(alfabeto_ma)];
                }
            }
            strcpy(ptr[n].senha, temp);
            salvar_senha(&ptr[n]);
            //para listas encadeadas
            ptr = (password *) realloc(ptr, ((n)+1)*sizeof(password));
            if(ptr == NULL){
                printf("Erro de alocacao");
                exit(1);
            }
        break;
    } 

    free(ptr);
    printf("\n\t A senha foi criada com sucesso\n\n", ptr[n].senha);
    pause_ns(2);
    vanish();
}

int senha_valida(char *ptr){
    if(strlen(ptr) < 8){
        printf("            ERRO AO CRIAR A SENHA!\n");
        printf("A senha precisa ter pelo menos 8 caracteres.\n");
        return 0;
    }

    //guardando os caracteres em cada variavel para comparar depois
    int* count_alfabeto_min = caracteres_da_string(alfabeto_min);
    int* count_alfabeto_ma = caracteres_da_string(alfabeto_ma);
    int* count_numeros_e_especiais = caracteres_da_string(numeros_e_especiais);

    //guarda o numero de caracteres de cada tipo
    int no_alfabeto_min = 0, no_alfabeto_ma = 0, no_numeros_e_especiais = 0;
    for(int i = 0; i < strlen(ptr); i++){
        //verifica se o char ptr[i] esta em alfabeto_min
        if(count_alfabeto_min[ptr[i]]) no_alfabeto_min++; 
        //verifica se o char ptr[i] esta em alfabeto_ma
        else if(count_alfabeto_ma[ptr[i]]) no_alfabeto_ma++; 
        //verifica se o char ptr[i] esta nemnumeros_e_especiais
        else if(count_numeros_e_especiais[ptr[i]]) no_numeros_e_especiais++;
        else{
            printf("   ERRO AO CRIAR A SENHA!\n");
            printf("Caractere %c Invalido\n", ptr[i]);
            return 0;
        }

    }

    if(!no_alfabeto_min){
        printf("              ERRO AO CRIAR A SENHA!\n");
        printf("A senha precisa ter pelo menos 1 letra minuscula.\n");
        return 0;
    }

    if(!no_alfabeto_ma){
        printf("              ERRO AO CRIAR A SENHA!\n");
        printf("A senha precisa ter pelo menos 1 letra maiuscula.\n");
        return 0;
    }

    if(!no_numeros_e_especiais){
        printf("                             ERRO AO CRIAR A SENHA!\n");
        printf("A senha precisa ter pelo menos "); 
        printf("1 numero ou caractere especial(@, #, $, %%, ou &).\n");
        return 0;
    }

    return 1;
}

void salvar_senha(password *ptr){
    FILE *fptr;
    fptr = fopen("senhas_salvas.bin", "ab");
    if(fptr == NULL) {
    printf( "Erro na abertura do arquivo");
    exit(0);
    }
    //como estavos identificando a finalidade e a senha pelo núemro da linha
    //temos que pular uma linha entre a escrita dos dois
    if(fwrite(&ptr[n].finalidade, sizeof(char), strlen(ptr[n].finalidade), fptr) != strlen(ptr[n].finalidade)){
        printf("Erro na escrita do arquivo!\n");
        exit(0);
    }
    //ch tem o valor de '\n', por isso é importante voltar o valor de ch
    //apos usar a funcao fread
    if(fwrite(&ch, sizeof(char), 1, fptr) != 1){
        printf("Erro na escrita do arquivo!\n");
        exit(0);
    }
    if(fwrite(&ptr[n].senha, sizeof(char), strlen(ptr[n].senha), fptr) != strlen(ptr[n].senha)){
        printf("Erro na escrita do arquivo!\n");
        exit(0);
    }
    if(fwrite(&ch, sizeof(char), 1, fptr) != 1){
        printf("Erro na escrita do arquivo!\n");
        exit(0);
    }

    fclose(fptr);
}

void lista_vazia(FILE *fptr){
    int op_menu;
    if(!count_line()){
        do{
            printf("\t\t----------------\n");
            printf("\t\t|LISTAGEM VAZIA|\n");
            printf("\t\t----------------\n");
            printf("1 - Criar senha\n");
            printf("2 - Sair\n");
            scanf(" %d", &op_menu);
            if(op_menu == 2){
                for(int i = 0; i < n; i++){
                    free(ptr[i].senha);
                    free(ptr[i].finalidade);
                }
                vanish();
                printf("\n\tO Programa Foi Finalizado!\n\t");
                exit(0);
            }
            if(op_menu != 1){
                printf("opcao invalida\n");
                getchar();
            }
        } while(op_menu != 1);
        criar_senha();
    }
    return;
}

void pause_ns(int n){
    t1 = t2 = time(NULL);
    
    while(t2 - t1 < n){
        t2 = time(NULL);
    }
    return;
}
void vanish(){
    if(sistema == 1){
        system("cls");
    }
    else if(sistema == 2){
        system("clear");
    }
}
