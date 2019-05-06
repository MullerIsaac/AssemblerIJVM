#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int tamanhoArq = 20; // tamanho em bytes da inicialização do programa
int numVar = 0; //numero de variaveis
char opcodes[20][15] = {"nop", "iadd", "isub", "iand", "ior", "dup", "pop", "swap", "bipush", "iload", "istore", "wide", "ldc_w", "iinc", "goto", "iflt", "ifeq", "if_icmpeq", "invokevirtual", "ireturn"};


int isOP(char *ch){
    int i;
    for(i = 0; i<=20; i++){
        if(strcmp(ch, opcodes[i]) == 0)
            return 1;
    }
    return 0;
}

void tamanhoProg(char *opcode){
    if(strcmp(opcode, "bipush") == 0 || strcmp(opcode, "iload") == 0 || strcmp(opcode, "istore") == 0)
        tamanhoArq += 2;
    else if(strcmp(opcode, "if_icmpeq") == 0 || strcmp(opcode, "goto") == 0)
        tamanhoArq += 3;
    else
        tamanhoArq += 1;
}

int tamanhoOperando(char *opcode){
    if(strcmp(opcode, "bipush") == 0 || strcmp(opcode, "iload") == 0 || strcmp(opcode, "istore") == 0)
        return 1;

    else if(strcmp(opcode, "if_icmpeq") == 0 || strcmp(opcode, "goto") == 0)
        return 2;

    else
        return 0;

}

int isInteger(char *ch){
    if(atoi(ch) == 0)
        return 0;
    return 1;
}

void trataVariavel(char *varNome){

}

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

int main()
{
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
                        numVar++;
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
    printf("%d\n", distLabel(arq, "l1"));
    printf("%d\n", numVar);
    printf("%d", tamanhoArq);
    fclose(file);
    return 0;


}
