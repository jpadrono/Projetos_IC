#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define NO_DE_CARACTERES 256

//lembrar de tirar os acentos e ?
//verificar se todos os allocs foram livres
//verificar se todos os arquivos foram fechados

typedef struct _password{
    char finalidade[50];
    char senha[100];
    //para listas encadeadas
    struct _password *ant;
    struct _password *prox; 
} password;

password *ptr_inicio = NULL, *ptr;

void salvar_senha(password *ptr);
void criar_senha();
void apresentar_senha();
void apagar_senha();
int senha_valida(char *ptr);
int n = 0, tamanho_da_senha = 16;
char ch = '\n';

//strings para geracao da senha automatica e validacao da senha manual (sem ?)
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

    /*******************************************************************
    * seta a seed da funcao rand como o tempo desde 01/01/1970         *
    * assim para cada execucao teremos uma nova seed                   *
    * interessante pensar em formas mais seguras de gerar uma seed     *
    * pois sabendo o tempo exato em que o programa foi rodado          *
    * torna possivel recriar a senha gerada                            *
    *******************************************************************/
    srand(time(NULL));
    int op_menu = 0;
    // Tela de selecao com as e escolhas
    // A tela ira aparecer ate que o usuario esoclha uma opcao existente
    do{
        printf("1 - Criar nova senha\n");
        printf("2 - Apresentar senha salva\n");
        printf("3 - Apagar senha salva\n");
        printf("4 - sair\n");
        scanf("%d", &op_menu);
        if(op_menu == 4) return 0;
        if(op_menu != 1 && op_menu != 2 && op_menu != 3) printf("opcao invalida\n");
    } while(op_menu != 1 && op_menu != 2 && op_menu != 3);


   switch(op_menu){
        case 1:
            criar_senha();
            break;
        
        case 2:
            printf("\nLista de Finalidades:\n\n");
            apresentar_senha();
            break;
    /*    
        case 3:
            apagar_senha();
            break;*/
    }

    for(int i = 0; i < n; i++){
        free(ptr[i].senha);
        free(ptr[i].finalidade);
    }
    return 0;
}

void apresentar_senha(){
    FILE *fptr;
    fptr = fopen("senhas_salvas.txt", "rb");

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
    
    //conta o n?mero de linhas do arquivo
    while(!feof(fptr)){
        ch = fgetc(fptr);
        if(ch == '\n') count_end_line++;   
    }
    // reseta o ponteiro para o inicio do arquivo
    rewind(fptr);

    int n_line = 0;
    //reseta o ch
    ch = '\n';
    while(!feof(fptr)){
        if((n_line+1)&1 && ch == '\n' && n_line < count_end_line){                              
            printf("%d - ", (n_line+2)/2);
        }
        ch = fgetc(fptr);
        if(!(n_line&1)){                                             
            printf("%c", ch);                                                
        }                                                                    
        if(ch == '\n') n_line++;
    }

    rewind(fptr);
    n_line = 0;
    //inicio do processo para localiza??o e escrita da senha
    printf("\nDigite o numero da senha a ser exibida: ");
    int escolha;
    scanf("%d", &escolha);

    while(!feof(fptr)){
        ch = fgetc(fptr);
        //printa a finalidade para -2 e senha para -1
        if(n_line == 2*escolha-2 || n_line == 2*escolha-1){
            printf("%c", ch);
        }
        if(ch == '\n') n_line++;
        
    }

    fclose(fptr);


}

void criar_senha(){

    password *ptr = NULL;
    // Tela de selecao com a escolha da formacao da senha
    int op_senha;
    do{
        printf("1 - Digitacao manual\n");
        printf("2 - Geracao automatica\n");
        scanf(" %d", &op_senha);
        if(op_senha != 1 && op_senha != 2) printf("Opcao invalida\n");
    }while(op_senha != 1 && op_senha != 2);

    //aloca memoria para um novo no
    ptr = (password *) malloc(sizeof(password));

    *ptr[n].finalidade = NULL;
    printf("Digite a finalidade da senha: ");
    scanf(" %[^\n]", ptr[n].finalidade);

    switch(op_senha){
        case 1:
            printf("A senha deve ter no minimo:\n");
            printf("8 caracteres\n");
            printf("1 letra maiuscula e 1 miuúscula\n");
            printf("1 caractere numerico ou especial(@, #, $, %%, ou &)\n");
            do{
                
                printf("Digite a senha: ");
                scanf(" %[^\n]", ptr[n].senha);
            } while (!senha_valida(ptr[n].senha));
            salvar_senha(&ptr[n]);
            printf("A senha criada foi: %s\n", ptr[n].senha);

            //para listas encadeadas
            ptr = (password *) realloc(ptr, ((n++)+1)*sizeof(password));
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
            printf("A senha gerada foi: %s\n", ptr[n].senha);
            //para listas encadeadas
            ptr = (password *) realloc(ptr, ((n++)+1)*sizeof(password));
            if(ptr == NULL){
                printf("Erro de alocacao");
                exit(1);
            }
        break;
    } 

    free(ptr);
}

int senha_valida(char *ptr){
    if(strlen(ptr) < 8){
        printf("a senha precisa ter pelo menos 8 caracteres.\n");
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
    }

    if(!no_alfabeto_min){
        printf("A senha precisa ter pelo menos 1 letra minuscula.\n");
        return 0;
    }

    if(!no_alfabeto_ma){
        printf("A senha precisa ter pelo menos 1 letra maiuscula.\n");
        return 0;
    }

    if(!no_numeros_e_especiais){
        printf("A senha precisa ter pelo menos "); 
        printf("1 numero ou caractere especial(@, #, $, %%, ou &).\n");
        return 0;
    }
    if((no_alfabeto_min+no_alfabeto_ma+no_numeros_e_especiais)!=strlen(ptr)){
        printf("A senha posssui um caractere invalido.\n");
        return 0;
    }

    return 1;
}

void salvar_senha(password *ptr){
    FILE *senhas_salvas;
    senhas_salvas = fopen("C:\\Users\\Lenovo\\programas vscode\\IME\\gerenciador de senhas\\senhas_salvas.txt", "ab");
    if(senhas_salvas == NULL) {
    printf( "Erro na abertura do arquivo");
    exit(0);
    }
    fputs(ptr[n].finalidade, senhas_salvas);
    fputs("\n", senhas_salvas);
    fputs(ptr[n].senha, senhas_salvas);
    fputs("\n", senhas_salvas);

    fclose(senhas_salvas);
    return 0;
    

}