#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int tamanhoArq = 20; // tamanho em bytes da inicialização do programa
int numVar = 0; //numero de variaveis
char opcodes[20][15] = {"nop", "iadd", "isub", "iand", "ior", "dup", "pop", "swap", "bipush", "iload", "istore", "wide", "ldc_w", "iinc", "goto", "iflt", "ifeq", "if_icmpeq", "invokevirtual", "ireturn"};
char variaveis[128][20];

int isOP(char *);
void tamanhoProg(char *);
int tamanhoOperando(char *);
int isInteger(char *);
int retornaVariavel(char *);
void trataVariavel(char* );
void inicializaVetorVar();
int distLabel(char*, char*);

int main(int argc, char *argv[])
{
    inicializaVetorVar();
    char arq[] = "assembly.txt";
    FILE *file;
    file = fopen(arq, "r");
    char ch[100], op1[50], op2[50], op3[50];

    if (file == NULL){
        printf("Não foi possível abrir o arquivo");
        return 0;
    }


    while((fgets(ch, sizeof(ch), file))!= NULL){
        sscanf(ch, "%s", op1);
        if(isOP(op1)){
            tamanhoProg(op1);
            if(tamanhoOperando(op1) > 0 ){
                sscanf(ch, "%s %s", op1, op2);
                if(!isInteger(op2)){
                    if(tamanhoOperando(op1) == 1)
                        trataVariavel(op2);
                }
            }
        }
        else{
            sscanf(ch, "%s %s", op1, op2);
            tamanhoProg(op2);
            if(tamanhoOperando(op2))
                sscanf(ch, "%s %s %s", op1, op2, op3);
        }


        op1[0] = '\0';
        op2[0] = '\0';
        op3[0] = '\0';
    }
    printf("%d\n", retornaVariavel("k"));
    printf("%d\n", distLabel(arq, "l2"));
    printf("%d\n", numVar);
    printf("%d", tamanhoArq);
    fclose(file);
    return 0;


}

//Testa se a string é um opcode
int isOP(char *ch){
    int i;
    for(i = 0; i<=20; i++){
        if(strcmp(ch, opcodes[i]) == 0)
            return 1;
    }
    return 0;
}

//atualiza o tamanho do programa com base no opcode recebido
void tamanhoProg(char *opcode){
    if(strcmp(opcode, "bipush") == 0 || strcmp(opcode, "iload") == 0 || strcmp(opcode, "istore") == 0)
        tamanhoArq += 2;
    else if(strcmp(opcode, "if_icmpeq") == 0 || strcmp(opcode, "goto") == 0)
        tamanhoArq += 3;
    else
        tamanhoArq += 1;
}

//retorna o tamanho em bytes do operando do opcode recebido
int tamanhoOperando(char *opcode){
    if(strcmp(opcode, "bipush") == 0 || strcmp(opcode, "iload") == 0 || strcmp(opcode, "istore") == 0)
        return 1;

    else if(strcmp(opcode, "if_icmpeq") == 0 || strcmp(opcode, "goto") == 0)
        return 2;

    else
        return 0;

}

//testa se a string pode ser um inteiro
int isInteger(char *ch){
    if(atoi(ch) == 0)
        return 0;
    return 1;
}

//retorna o indice da variavel
int retornaVariavel(char *var){
    int i;
    for(i = 0; i < numVar; i++){
        if(strcmp(var, variaveis[i])==0)
            return i;
    }
    return 0;
}

//recebe uma vaiavel e armazena no vetor de variaveis
void trataVariavel(char *varNome){
    int i;
    for(i = 0; i < numVar; i++){
        if(strcmp(varNome, variaveis[i]) == 0)
            return;
    }
    *variaveis[i] = *varNome;
    numVar++;

}

void inicializaVetorVar(){
    int i;
    for(i=0; i<128; i++){
        *variaveis[i] = '\0';
    }
}

//retorna a distancia do label recebido
int distLabel(char *arq, char* label){

    FILE *file;
    file = fopen(arq, "r");
    char ch[100], op1[50], op2[50];
    int achou = 0;
    int i = 0;

    while((fgets(ch, sizeof(ch), file))!= NULL){
        sscanf(ch,"%s", op1);
        if(isOP(op1)){
            if(tamanhoOperando(op1) > 0){
                sscanf(ch,"%s %s", op1, op2);
                if(tamanhoOperando(op1)== 2){
                    if(strcmp(op2, label) == 0 && achou==0){
                            achou = 1;
                            i+=2;
                    }
                    else if(achou == 1)
                        i+= 3;
                }
                else if(tamanhoOperando(op1)== 1 && achou == 1)
                    i+=2;
                else{
                    if(achou == 1)
                        i+=1;
                }
            }else if(achou == 1)
                i+=1;
        }else{
            sscanf(ch, "%s %s", op1, op2);
            if(strcmp(op1, label)== 0)
                return i + 1;
            else if(tamanhoOperando(op2)== 1 && achou == 1)
                i+=2;
            else if(tamanhoOperando(op2)== 0 && achou == 1)
                i+=1;
        }
        op1[0] = '\0';
        op2[0] = '\0';
    }
    fclose(file);

}


