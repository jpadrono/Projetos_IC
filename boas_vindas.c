#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum meses{jan = 1, feb, mar, apr, may, jun, jul, aug,
sep, oct, nov, dec};
int v[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
char mesess[12][4] = {"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", 
"sep", "oct", "nov", "dec"};

typedef struct{
    char nome[50];
    int dia, mes, ano;
}Contato;

void atualizar(int n, char *dia, char *mes){
    FILE *abc, *abc_aux;
    char ch, ch_aux;
    int linha = 1, variavel = 0;

    abc = fopen("aniversarios.bin", "rb");
    abc_aux = fopen("aniversarios_aux.bin", "wb");

    while(!feof(abc)){
        fread(&ch, sizeof(char), 1, abc);
        if(linha != n+1){
            fwrite(&ch, sizeof(char), 1, abc_aux);
        }
        else{
            if(variavel == 0){
                fwrite(&dia, sizeof(char), strlen(dia), abc_aux);
                fwrite(&mes, sizeof(char), strlen(mes), abc_aux);
                variavel++;
            }
        }
        if(ch == '\n') linha++;
    }

    fclose(abc);
    fclose(abc_aux);
    abc = fopen("aniversarios.bin", "wb");
    abc_aux = fopen("aniversarios_aux.bin", "rb");

    while(!feof(abc_aux)){
        fread(&ch, sizeof(char), 1, abc_aux);
        fwrite(&ch, sizeof(char), 1, abc);
    }
    fclose(abc);
    fclose(abc_aux);
}

void atualizarp(int n){
    FILE *abc, *abc_aux;
    char ch, ch_aux;
    int linha = 1, variavel = 0;

    abc = fopen("aniversarios.bin", "rb");
    abc_aux = fopen("aniversarios_aux.bin", "wb");

    while(!feof(abc)){
        fread(&ch, sizeof(char), 1, abc);
        if(linha != n || linha != n+1){
            fwrite(&ch, sizeof(char), 1, abc_aux);
        }
        if(ch == '\n') linha++;
    }

    fclose(abc);
    fclose(abc_aux);
    abc = fopen("aniversarios.bin", "wb");
    abc_aux = fopen("aniversarios_aux.bin", "rb");

    while(!feof(abc_aux)){
        fread(&ch, sizeof(char), 1, abc_aux);
        fwrite(&ch, sizeof(char), 1, abc);
    }
    fclose(abc);
    fclose(abc_aux);
}

void salvar(char *nome, char *dia, char *mes){
    // "ab" adiciona sem apagar o que já existe no arquivo
    // caso queria apgar o que já existe usaria "wb"
    // caso o arquivo n exista, ele é criado automaticamente no mesma pasta do programa
    // ele so le um arquivo ja existente se ele estiver na mesma pasta do programa
    FILE *file = fopen("aniversarios.bin", "a+b"); 
    if(file == NULL){
        printf("Erro na abertura do arquivo\n");
    }
    int i=0, n = 1;
    char barraN = '\n', var, cmp[50];
    while(!feof(file)){ //end of file, feof(file) me diz qunado o ponteito *file chegar ao final do arquivo
        fread(&var, sizeof(char), 1, file); 
        if(var == '\n'){
            int a = 0;
            for(int k = 0; k < strlen(nome); k++){
                if(nome[k] == cmp [k]) a++;
            }
            if(a == strlen(nome)){
                atualizar(n, dia, mes);
                printf("foo");
                return;
            }
            memset(cmp, 0 , 50); // pesquisamos no google como apagar a string
            n++;
            i = 0;
        }
        else cmp[i++] = var;
    }

    fclose(file);
    file = fopen("aniversarios.bin", "a+b");


    if(file != NULL){ 
        printf("boo");
        fwrite(&nome, sizeof(char), strlen(nome), file); //fwrite(void *buffer, size_t, count, FILE *ptr)
        fwrite(&barraN, sizeof(char), 1, file);
        fwrite(&dia, sizeof(char), strlen(dia), file);
        fwrite(&mes, sizeof(char), strlen(mes), file);
    }
    else
        printf("\n\tNAO FOI POSSIVEL ABRIR/CRIAR O ARQUIVO!\n");
    fclose(file);
    return;
}

void apagar(char *nome){
    // "ab" adiciona sem apagar o que já existe no arquivo
    // caso queria apgar o que já existe usaria "wb"
    // caso o arquivo n exista, ele é criado automaticamente no mesma pasta do programa
    // ele so le um arquivo ja existente se ele estiver na mesma pasta do programa
    FILE *file = fopen("aniversarios.bin", "ab"); 
    int i=0, n = 1;
    char barraN = '\n', var, cmp[50];
    while(!feof(file)){ //end of file, feof(file) me diz qunado o ponteito *file chegar ao final do arquivo
        fread(&var, sizeof(char), 1, file); 
        if(var == '\n'){
            int a = 0;
            for(int k = 0; k < strlen(nome); k++){
                if(nome[k] == cmp [k]) a++;
            }
            if(a == strlen(nome)){
                atualizarp(n);
                printf("foo");
                return;
            }
            memset(cmp, 0 , 50); // pesquisamos no google como apagar a string
            n++;
            i = 0;
        }
        else cmp[i++] = var;
    }
}

int n_dias(int dias){
    FILE *abc;
    char ch, string[5], fulano[50];
    int linha = 1, i = 0, j = 0, vetor[300], dia_aniv = 0, mes_aniv = 0;


    abc = fopen("aniversarios.bin", "rb");

    while(!feof(abc)){
        if(linha % 2 == 0){
            fread(&ch, sizeof(char), 1, abc);
            printf("%c", ch);
            if(ch == '\n'){
                for(int i = 0; i < 4; i++){
                    switch(i){
                        case 0:
                            dia_aniv += 10*(string[0] - '0');
                            break;
                        case 1:
                            dia_aniv += string[1] - '0';
                            break;
                        case 2:
                            mes_aniv += 10*(string[2] - '0');
                            break;
                        case 3:
                            mes_aniv += string[3] - '0';
                            break;    
                    }
                }
                for(int i = 1; i < mes_aniv; i++){
                    dia_aniv += v[i-1];
                }
                if(dia_aniv == dias){
                    printf("%s faz aniversario hoje! De-lhe os parabens\n", fulano);
                 }
                else if(dia_aniv - dias < 11){
                    printf("faltam %d dias para o aniversario de %s",
                            dia_aniv - dias, fulano);
                }
                linha++;
                i = 0;
            }
            string[i++] = ch;
        }
        else{
            fread(&ch, sizeof(char), 1, abc);
            if(ch == '\n'){
                linha++;
                j = 0;
            } 
            else{
                fulano[j++] = ch;
            }
        }
    }

}
int main(int argc, char *argv[]){
    if(argc == 1 ){ 
    FILE *arquivo = fopen("sunrise.txt", "r");
    if(arquivo == NULL){
        printf("ERRO\n");
        return 0;
    }
    char ch, mes[10];
    while(!feof(arquivo)){
        ch = getc(arquivo);
        printf("%c", ch);
    }
    printf("\n\n\t\t\tBEM VINDO(A)\n\n");
    struct tm *local;
    time_t t;
    char data_atual[50], mes_atual[4], dia_atual[3];
    int dias, mes_a;
    t = time(NULL); // retorna o tempo desde o dia 01/01/1970

    local = localtime(&t);

    strcpy(data_atual, asctime(local));

    for(int i = 0; i < 3; i++){
        mes_atual[i] = data_atual[4+i];
    }

    for(int i = 0; i < 12; i++){
        int b = 0;
        for(int j = 0; j < 3; j++){
            if(data_atual[4+j] == mesess[i][j]) b++;
        }
        if(b == 3){
            mes_a = i+1;
        }
    }

    for(int i = 0; i < 2; i++){
        dia_atual[i] = data_atual[8+i];
    }

    for(int i = 1; i < mes_a; i++){
        dias += v[i-1];
    }
    dias += (dia_atual[0] - '0')*10 + (dia_atual[1] - '0');


    n_dias(dias);

}
//(dia_atual[0] - '0')*10 + dias_atual[1] - '0';
    if(!strcmp(argv[1], "i")){
    if(argc != 5){
        printf("erro\n");
        return 0; 
    }
    salvar(argv[2], argv[3], argv[4]);
}
    else if(!strcmp(argv[1], "d")){
        if(argc != 3){
            printf("erro\n");
            return 0;
        }
        apagar(argv[2]);
    }
    else{   
    printf("ERRO NA EXECUSSO, CHAMADA INVALIDA\n");
    return 0;
}
}
