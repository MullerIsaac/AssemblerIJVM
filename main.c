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

int tamanhoProg(char *opcode){
    if(strcmp(opcode, "bipush") == 0 || strcmp(opcode, "iload") == 0 || strcmp(opcode, "istore") == 0){
        tamanhoArq += 2;
        return 1;
    }
    else if(strcmp(opcode, "if_icmpeq") == 0 || strcmp(opcode, "goto") == 0){
        tamanhoArq += 3;
        return 2;
    }
    else{
        tamanhoArq += 1;
        return 0;
    }
}

int isInteger(char *ch){
    size_t i = 0;
    size_t len = strlen(ch);
    while(i < len){
        if(!(isdigit(ch[i])))
            return 0;
        i++;
    }
    return 1;
}

int main()
{
    FILE *file;
    file = fopen("assembly.txt", "r");
    char ch[100], op1[50], op2[50], op3[50];

    if (file == NULL){
        printf("Não foi possível abrir o arquivo");
        return 0;
    }


    while((fgets(ch, sizeof(ch), file))!= NULL){
        sscanf(ch, "%s", op1);
        if(isOP(op1)){
            int x = tamanhoProg(op1);
            if(x){
                sscanf(ch, "%s %s", op1, op2);
                if(!isInteger(op2)){
                    if(x == 1)
                        numVar += 1;
                }
            }
        }
        else{
            sscanf(ch, "%s %s", op1, op2);
            int x = tamanhoProg(op2);
            if(x)
                sscanf(ch, "%s %s %s", op1, op2, op3);

        }


        op1[0] = '\0';
        op2[0] = '\0';
        op3[0] = '\0';
    }
    printf("%d\n", numVar);
    printf("%d", tamanhoArq);
    fclose(file);
    return 0;


}
