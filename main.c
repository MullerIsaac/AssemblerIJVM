#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int tamanhoArq = 20; // tamanho em bytes da inicialização do programa
int numVar = 0; //numero de variaveis
char opcodes[20][15] = {"nop", "iadd", "isub", "iand", "ior", "dup", "pop", "swap", "bipush", "iload", "istore", "wide", "ldc_w", "iinc", "goto", "iflt", "ifeq", "if_icmpeq", "invokevirtual", "ireturn"};
char variaveis[256][20];

int isOP(char *);
void tamanhoProg(char *);
int tamanhoOperando(char *);
int isInteger(char *);
int retornaVariavel(char *);
void trataVariavel(char* );
void inicializaVetorVar();
int distLabel(char*, char*);
void primeiraVarredura(char*);
int retornaCodigo(char* );
void escreverBin(int*, int, char*);

int main(int argc, char *argv[])
{

    char arq[] = "assembly.txt";
    char dest[] = "assembly.exe";
    inicializaVetorVar();
    primeiraVarredura(arq);


    unsigned long int init = 0x7300;
    unsigned long int cpp = 0x0006;
    unsigned long int lv = 0x1001;
    unsigned long int sp = lv+numVar;
    unsigned long int pc = 0x0400;
    char linha[100], op1[30], op2[30], op3[30];
    int aux, param;




    FILE *origem;
    origem = fopen(arq, "r");

    FILE *destino;
    destino = fopen(dest, "wb");

    fwrite(&tamanhoArq, sizeof(unsigned long int), 1, destino);
    fwrite(&init, sizeof(unsigned long int), 1, destino);
    fwrite(&cpp, sizeof(unsigned long int), 1, destino);
    fwrite(&lv, sizeof(unsigned long int), 1, destino);
    fwrite(&pc, sizeof(unsigned long int), 1, destino);
    fwrite(&sp, sizeof(unsigned long int), 1, destino);


    //fazer uma função para escrever

    while(fgets(linha, 100, origem) != NULL){
        sscanf(linha, "%s %s %s", op1, op2, op3);
        if(isOP(op1)){
            aux = retornaCodigo(op1);
            fwrite(&aux, sizeof(unsigned char), 1, destino);
            if(tamanhoOperando(op1)== 1){
                if(isInteger(op2)){
                    param = atoi(op2);
                    fwrite(&param, sizeof(unsigned char), 1, destino);
                }else{
                    param = retornaVariavel(op2);
                    fwrite(&param, sizeof(unsigned char), 1, destino);
                }

            }else if(tamanhoOperando(op1) == 2){
                param = distLabel(arq, op2)*256;
                fwrite(&param, sizeof(unsigned short int), 1, destino);
            }

        }else{
            aux = retornaCodigo(op2);
            fwrite(&aux, sizeof(unsigned char), 1, destino);
            if(tamanhoOperando(op2)== 1){
                if(isInteger(op3)){
                    param = atoi(op3);
                    fwrite(&param, sizeof(unsigned char), 1, destino);;
                }else{
                    param = retornaVariavel(op3);
                    fwrite(&param, sizeof(unsigned char), 1, destino);
                }

            }

        }
        op1[0] = '\0';
        op2[0] = '\0';
        op3[0] = '\0';

    }
    fclose(destino);
    fclose(origem);

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
    size_t i = 0;
    size_t len = strlen(ch);
    while(i<len){
        if(!isdigit(ch[i]))
            return 0;
        i++;
    }
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
    for(i=0; i<256; i++){
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
    return 0;

}

void primeiraVarredura(char *arq){

    FILE *file;
    file = fopen(arq, "r");
    char ch[100], op1[50], op2[50], op3[50];

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

    fclose(file);

}

int retornaCodigo(char * opcode){

    if(strcmp(opcode, "nop") == 0){
        return 0x01;
    }
    else if(strcmp(opcode, "iadd") == 0){
        return 0x02;
    }
    else if(strcmp(opcode, "isub") == 0){
        return 0x05;
    }
    else if(strcmp(opcode, "iand") == 0){
        return 0x08;
    }
    else if(strcmp(opcode, "ior") == 0){
        return 0x0b;
    }
    else if(strcmp(opcode, "bipush") == 0){
        return 0x19;
    }
    else if(strcmp(opcode, "iload") == 0){
        return 0x1c;
    }
    else if(strcmp(opcode, "istore") == 0){
        return 0x22;
    }
    else if(strcmp(opcode, "goto") == 0){
        return 0x3c;
    }
    else if(strcmp(opcode, "if_icmpeq") == 0){
        return 0x4b;
    }
    return 0;

}



