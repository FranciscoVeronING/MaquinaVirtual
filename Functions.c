#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Functions.h"

void MOV(struct VM* mv, int opA_content, int opB_content, char opA, char opB){
    switch (opA) {
        case 0b00: {
            printf("entro a set memoria ");
            set_memoria(get_puntero(opA_content, *mv), value_op(opB_content, opB,*mv), mv);       //memoria
            break;
        }
        case 0b10: {
            printf("entro a set registro ");
            set_registro(opA_content, value_op(opB_content, opB, *mv), mv);      //registro
            break;
        }
    }
}

void ADD(struct VM* mv, int opA_content, int opB_content , char opA, char opB) {
    // Obtenemos el valor del operando A y B
    int value_A = value_op(opA_content, opA, *mv);
    int value_B = value_op(opB_content, opB, *mv);

    value_A += value_B;

    // Guardamos el valor resultante de vuelta en el operando A
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA,value_A, mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = 0X10000000;
    else if(value_A == 0)
        mv->registers_table[8] = 0x01000000;
}

void SUB(struct VM* mv, int opA_content, int opB_content , char opA, char opB) {
    // Obtenemos el valor del operando A y B
    int value_A = value_op(opA_content, opA, *mv);
    int value_B = value_op(opB_content, opB, *mv);

    value_A -= value_B;

    // Guardamos el valor resultante de vuelta en el operando A
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA,value_A,mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = 0X10000000;
    else if(value_A == 0)
        mv->registers_table[8] = 0x01000000;
}

void MUL(struct VM* mv, int opA_content, int opB_content , char opA, char opB) {
    // Obtenemos el valor del operando A y B
    int value_A = value_op(opA_content, opA, *mv);
    int value_B = value_op(opB_content, opB, *mv);

    value_A *= value_B;

    // Guardamos el valor resultante de vuelta en el operando A
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA,value_A,mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = 0X10000000;
    else if(value_A == 0)
        mv->registers_table[8] = 0x01000000;
}

void DIV(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A = value_op(opA_content, opA, *mv);
    int value_B = value_op(opB_content, opB, *mv);

    if(value_B != 0) {
        int resto = value_A;
        value_A /= value_B;
        resto %= value_B;
        mv->registers_table[8] = resto;  // se guarda el valor del resto de la division entera
        // Guardamos el valor resultante de vuelta en el operando A
        if (opA == 0) { // Si el operando A es de memoria
            int pointer = get_puntero(opA_content, *mv);
            set_memoria(pointer, value_A, mv);
        } else if (opA == 2)  // Si el operando A es un registro
            set_registro(opA, value_A, mv);

        //cambiamos el valor del CC(condition code
        if(value_A < 0)
            mv->registers_table[8] = 0X10000000;
        else if(value_A == 0)
            mv->registers_table[8] = 0x01000000;
    } else
        *error = 3;


}

void SWAP(struct VM* mv, int opA_content, int opB_content , char opA, char opB) {
    // Obtenemos el valor del operando A y B
    int value_A = value_op(opA_content, opA, *mv);
    int value_B = value_op(opB_content, opB, *mv);

    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_B, mv);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA,value_B, mv);

    if(opB == 0) { // Si el operando B es de memoria
        int pointer = get_puntero(opB_content, *mv);
        set_memoria(pointer, value_A, mv);
    }
    else if(opB == 2)  // Si el operando B es un registro
        set_registro(opB,value_A, mv);
}

void AND(struct VM* mv, int opA_content, int opB_content , char opA, char opB) {
    // Obtenemos el valor del operando A y B
    int value_A = value_op(opA_content, opA, *mv);
    int value_B = value_op(opB_content, opB, *mv);

    value_A &= value_B;

    // Guardamos el valor resultante de vuelta en el operando A
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA,value_A,mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = 0X10000000;
    else if(value_A == 0)
        mv->registers_table[8] = 0x01000000;
}

void OR(struct VM* mv, int opA_content, int opB_content , char opA, char opB) {
    // Obtenemos el valor del operando A y B
    int value_A = value_op(opA_content, opA, *mv);
    int value_B = value_op(opB_content, opB, *mv);

    value_A |= value_B;

    // Guardamos el valor resultante de vuelta en el operando A
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA,value_A,mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = 0X10000000;
    else if(value_A == 0)
        mv->registers_table[8] = 0x01000000;
}

void XOR(struct VM* mv, int opA_content, int opB_content , char opA, char opB) {
    // Obtenemos el valor del operando A y B
    int value_A = value_op(opA_content, opA, *mv);
    int value_B = value_op(opB_content, opB, *mv);

    value_A ^= value_B;

    // Guardamos el valor resultante de vuelta en el operando A
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA,value_A,mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = 0X10000000;
    else if(value_A == 0)
        mv->registers_table[8] = 0x01000000;
}

void CMP(struct VM* mv, int opA_content, int opB_content , char opA, char opB) {
    // Obtenemos el valor del operando A y B
    int value_A = value_op(opA_content, opA, *mv);
    int value_B = value_op(opB_content, opB, *mv);

    value_A -= value_B;
    //cambiamos el valor del CC(condition code)
    if(value_A < 0)
        mv->registers_table[8] = 0X10000000;
    else if(value_A == 0)
        mv->registers_table[8] = 0x01000000;
}

void SHL(struct VM* mv, int opA_content, int opB_content , char opA, char opB) {
    // Obtenemos el valor del operando A y B
    int value_A = value_op(opA_content, opA, *mv);
    int value_B = value_op(opB_content, opB, *mv);

    // Realizamos el desplazamiento lógico a la izquierda en A por el número de posiciones indicado en B
    value_A <<= value_B;

    // Guardamos el valor resultante de vuelta en el operando A
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA,value_A,mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = 0X10000000;
    else if(value_A == 0)
        mv->registers_table[8] = 0x01000000;
}

void SHR(struct VM* mv, int opA_content, int opB_content , char opA, char opB) {
    // Obtenemos el valor del operando A y B
    int value_A = value_op(opA_content, opA, *mv);
    int value_B = value_op(opB_content, opB, *mv);

    // Realizamos el desplazamiento lógico a la izquierda en A por el número de posiciones indicado en B
    value_A >>= value_B;

    // Guardamos el valor resultante de vuelta en el operando A
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA,value_A,mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = 0X10000000;
    else if(value_A == 0)
        mv->registers_table[8] = 0x01000000;
}

void RND(struct VM* mv, int opA_content, int opB_content , char opA, char opB) {
    // Obtenemos el valor del operando A y B
    int value_A = value_op(opA_content, opA, *mv);
    int value_B = value_op(opB_content, opB, *mv);
    srand(time(NULL));
    value_A =   rand()%value_B; ///buscar funcion random!

    // Guardamos el valor resultante de vuelta en el operando A
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA,value_A,mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = 0X10000000;
    else if(value_A == 0)
        mv->registers_table[8] = 0x01000000;
}


void JMP(struct VM* mv, int opA_content, char opA, int *error){
    int value_A = value_op(opA_content, opA, *mv);
    if(value_A < mv->segment_descriptor_table[0].size)
        mv->registers_table[5]  = (int)(value_A & 0x0000FFFF);
    else
        *error = 2; //caida de segmento
}

void JZ(struct VM* mv, int opA_content, char opA, int *error){
    int aux = (mv->registers_table[8] >> 28) & 0x0000000F;
    if( aux == 0x1) {
        int value_A = value_op(opA_content, opA, *mv);
        if (value_A < mv->segment_descriptor_table[0].size)
            mv->registers_table[5] = (int) (value_A & 0x0000FFFF);
        else
            *error = 2; //caida de segmento
    }
}
void JP(struct VM* mv, int opA_content, char opA, int *error){
    int aux = (mv->registers_table[8] >> 28) & 0x0000000F;
    if( aux == 0x0) {
        int value_A = value_op(opA_content, opA, *mv);
        if (value_A < mv->segment_descriptor_table[0].size)
            mv->registers_table[5] = (int) (value_A & 0x0000FFFF);
        else
            *error = 2; //caida de segmento
    }
}

void JN(struct VM* mv, int opA_content, char opA, int *error){
    int aux = (mv->registers_table[8] >> 28) & 0x0000000F;
    if( aux == 0x2) {
        int value_A = value_op(opA_content, opA, *mv);
        if (value_A < mv->segment_descriptor_table[0].size)
            mv->registers_table[5] = (int) (value_A & 0x0000FFFF);
        else
            *error = 2; //caida de segmento
    }
}
void JNZ(struct VM* mv, int opA_content, char opA, int *error){
    int aux = (mv->registers_table[8] >> 28) & 0x0000000F;
    if( aux == 0x2 || aux == 0x0) {
        int value_A = value_op(opA_content, opA, *mv);
        if (value_A < mv->segment_descriptor_table[0].size)
            mv->registers_table[5] = (int) (value_A & 0x0000FFFF);
        else
            *error = 2; //caida de segmento
    }
}

void JNP(struct VM* mv, int opA_content, char opA, int *error){
    int aux = (mv->registers_table[8] >> 28) & 0x0000000F;
    if( aux == 0x1 || aux == 0x2) {
        int value_A = value_op(opA_content, opA, *mv);
        if (value_A < mv->segment_descriptor_table[0].size)
            mv->registers_table[5] = (int) (value_A & 0x0000FFFF);
        else
            *error = 2; //caida de segmento
    }
}

void JNN(struct VM* mv, int opA_content, char opA, int *error){
    int aux = (mv->registers_table[8] >> 28) & 0x0000000F;
    if( aux == 0x1 || aux == 0x0) {
        int value_A = value_op(opA_content, opA, *mv);
        if (value_A < mv->segment_descriptor_table[0].size)
            mv->registers_table[5] = (int) (value_A & 0x0000FFFF);
        else
            *error = 2; //caida de segmento
    }
}

void LDL(struct VM* mv, int opA_content, char opA){
    int value = value_op(opA_content, opA, *mv);
    mv->registers_table[9] = value  & 0x0000FFFF;

}
void LDH(struct VM* mv, int opA_content, char opA){
    int value = value_op(opA_content, opA, *mv);
    mv->registers_table[9] = value  << 16;
}
void NOT(struct VM* mv, int opA_content, char opA){             //PREGUNTAR
    int value = value_op(opA_content, opA, *mv);
    value = ~value;
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value, mv);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA,value, mv);

    if(value < 0)
        mv->registers_table[8] = 0X10000000;
    else if(value == 0)
        mv->registers_table[8] = 0x01000000;

}

void STOP(int *error){
   *error =  -1;
};


int get_puntero(int op_content,struct VM mv){
    int pointer;
    ///00000 relleno xxxx cod_reg  yyyyyyyy yyyyyyyy offset
    char index = (char)((op_content >> 16) & 0x0F); // cod registro
   // printf("\n %x codigo de regstro en setpuntero\n",index);
    pointer = mv.registers_table[index];
   // printf(" \n %X este es pointer\n", pointer);
    pointer += (op_content & 0x00FFFF);

    printf(" \n %X este es pointer\n", pointer);
    return pointer;
}

void set_memoria(int pointer, int value, struct  VM* mv){
    /// el puntero tiene 2 bytes a codigo de segmento y 2 bytes de offset
    printf("estamos en set memoria\n");

    ///

    int index = pointer & 0x0000FFFF;//solo ponemos como index el offset delpuntero, en getpuntero hicimos la suma de os 2 offsets

    (*mv).memory[index] = (char) ((value & 0xff000000) >> 24);
    index ++;
    (*mv).memory[index ] = (char) ((value & 0x00ff0000) >>16);
    index ++;
    (*mv).memory[index ] = (char) ((value & 0x0000ff00) >>8);
    index ++;
    (*mv).memory[index] = (char) (value & 0x000000ff);
       //printf("%x\t",((*mv).memory[index]));

}

int get_memoria(int pointer, struct VM mv){
    ///hay 2 opciones, si es memoria directa, o si es la memoria de un registro
   //printf("\n entro a get memoria \n");
    int value = 0;
    int index = pointer & 0x0000FFFF; //OFFSET
    int aux = (pointer & 0xFFFF0000)>>16;
    //printf("\n %x aux \n", aux);
    //printf("\n %x tabla de descriptores de segmento tamaño \n", mv.segment_descriptor_table[aux].size);
    if(index <= (mv.segment_descriptor_table[aux].size - 4)){
        value = (int)(mv).memory[index];
        index+=1;
        value |= (mv).memory[index] << 8;
        index+=1;
        value |= (mv).memory[index] << 16;
        index+=1;
        value |= (mv).memory[index] << 24;
    }
    else{
        perror("te caiste del segmento pa");
    }
    //printf("\n %x value \n", value);
    return value;
}

void extract_op(int op_content, char *cod_reg, short int *offset){
    int mask = 0x000F0000;
    *cod_reg = (char)((op_content & mask)>>16);
    *offset = (short int)(op_content & 0x0000FFFF);
}

int value_op(int op_content, char op_type, struct VM mv){  //obtiene el valor del operando
    int value;
    switch(op_type){
        case 0: {   //caso de memoria
            //printf("\n entro a value_op caso memoria\n");
            char code_reg = (char) ((op_content & 0x0F0000) >> 16);
            //printf("\n %x codigo de registro \n", code_reg);
            int offset_1 = op_content & 0x00FFFF;
            //printf("\n %x offset 1 \n", offset_1);
            int offset_2 = mv.registers_table[code_reg] & 0x0000FFFF;
           // printf("\n %x offset 2 \n", offset_2);
            int pointer = (int)((mv.registers_table[code_reg] & 0xFFFF0000) + (offset_1 + offset_2));
            //printf("\n %x pointer \n", pointer);
            value = get_memoria(pointer, mv);
            break;
        }
        case 1:  //caso inmediato
            value = op_content;
            break;
        case 2: //caso registro
            value = get_registro(op_content,mv);
            break;
    }
    return value;
}

int get_registro(int op, struct VM mv){ //obtiene el valor de un registro
    int cod_reg, sec_reg, valor;
    sec_reg = op >> 4 & 0x3;        //almacena el tipo de registro
    cod_reg = op & 0xF;             //almacena el registro
    switch (sec_reg) {
        case 0:{
            valor = mv.registers_table[cod_reg];
            break;
        }
        case 1:{
            valor = mv.registers_table[cod_reg] & 0xFF;
            break;
        }
        case 2:{
            valor = mv.registers_table[cod_reg] & 0xFF00;
            break;
        }
        case 3:{
            valor = mv.registers_table[cod_reg] & 0xFFFF;
            break;
        }
    }
    return valor;
}

void set_registro(int op, int valor, struct VM* mv){
    //cambia el valor de un registro
    int cod_reg, sec_reg;
    sec_reg = op >> 4 & 0x3;        //almacena el tipo de registro
    cod_reg = op & 0xF;             //almacena el registro
    switch (sec_reg) {
        case 0:{
            (*mv).registers_table[cod_reg] = valor; //Caso EAX
            break;
        }
        case 1:{
            (*mv).registers_table[cod_reg] = ((*mv).registers_table[cod_reg] & 0xFFFFFF00) | valor; //se quieren mantener los 24 bits y modificar los ultimos 8 (caso AL)
            break;
        }
        case 2:{
            (*mv).registers_table[cod_reg] = ((*mv).registers_table[cod_reg] & 0xFFFF00FF) | (valor << 8); // Caso modificar AH
            break;
        }
        case 3:{
            (*mv).registers_table[cod_reg] = ((*mv).registers_table[cod_reg] & 0xFFFF0000) | valor; //Caso modificar AX
            break;
        }
    }
}

void llamado_funcion(struct VM* mv, char opA, int opA_content, char opB, int opB_content, char cod_op, int *error){

    switch (cod_op) {
        case 0:{
            printf("entra a mov");
            MOV(mv, opA_content, opB_content, opA, opB);
            break;
        }
        case 1:{
            printf("entra a ADD");
            ADD(mv,opA_content,opB_content,opA,opB);
            break;
        }
        case 2:{
            printf("entra a SUB");
            SUB(mv,opA_content,opB_content,opA,opB);
            break;
        }
        case 3:{
            printf("entra a SWAP");
            SWAP(mv,opA_content,opB_content,opA,opB);
            break;
        }
        case 4:{
            printf("entra a MUL");
            MUL(mv,opA_content,opB_content,opA,opB);
            break;
        }
        case 5:{
            printf("entra a DIV");
            DIV(mv,opA_content,opB_content,opA,opB, error);
            break;
        }
        case 6:{
            printf("entra a CMP");
            CMP(mv,opA_content,opB_content,opA,opB);
            break;
        }
        case 7:{
            printf("entra a SHL");
            SHL(mv,opA_content,opB_content,opA,opB);
            break;
        }
        case 8:{
            printf("entra a SHR");
            SHR(mv,opA_content,opB_content,opA,opB);
            break;
        }
        case 9:{
            printf("entra a AND");
            AND(mv,opA_content,opB_content,opA,opB);
            break;
        }
        case 0xA:{
            printf("entra a OR");
            OR(mv,opA_content,opB_content,opA,opB);
            break;
        }
        case 0xB:{
            printf("entra a XOR");
            XOR(mv,opA_content,opB_content,opA,opB);
            break;
        }
        case 0xC:{
            printf("entra a RND");
            RND(mv,opA_content,opB_content,opA,opB);
            break;
        }
       /* case 0x10:{
            printf("entra a SYS");
            SYS(mv, opB_content, opB_content,opA, opB);
            break;
        } */
        case 0x11:{
            printf("entra a JMP");
            JMP(mv,opB_content, opB, error);
            break;
        }
        case 0x12:{
            printf("entra a JZ");
            JZ(mv,opB_content, opB, error);
            break;
        }
        case 0x13:{
            printf("entra a JP");
            JP(mv,opB_content, opB, error);
            break;
        }
        case 0x14:{
            printf("entra a JN");
            JN(mv,opB_content, opB, error);
            break;
        }
        case 0x15:{
            printf("entra a JNZ");
            JNZ(mv,opB_content, opB, error);
            break;
        }
        case 0x16:{
            printf("entra a JNP");
            JNP(mv,opB_content, opB, error);
            break;
        }
        case 0x17:{
            printf("entra a JNN");
            JNN(mv,opB_content, opB, error);
            break;
        }
        case 0x18:{
            printf("entra a LDL");
            LDL(mv,opB_content, opB);
            break;
        }
        case 0x19:{
            printf("entra a LDH");
            LDH(mv,opB_content, opB);
            break;
        }
        case 0x1A:{
            printf("entra a NOT");
            NOT(mv,opB_content, opB);
            break;
        }
        case 0x1F:{
            printf("entra a STOP");
            STOP(error);
            break;
        }

    }
}