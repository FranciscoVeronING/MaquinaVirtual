#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void JMP(struct VM* mv, int opA_content, char opA){
    mv->registers_table[4]  = (int)(((*mv).registers_table[4] & 0xFFFF0000 )+(value_op(opA_content,opA,*mv) & 0x0000FFFF));
}



int get_puntero(int op_content,struct VM mv){
    int pointer ;
    ///00000 relleno xxxx cod_reg  yyyyyyyy yyyyyyyy offset
    char index = (char)(op_content >> 16); // cod registro
   // printf("\n %x codigo de regstro en setpuntero\n",index);
    pointer = mv.registers_table[index];
   // printf(" \n %X este es pointer\n", pointer);
    pointer += (op_content & 0x0000FFFF);

    //printf(" \n %X este es pointer\n", pointer);
    return pointer;
}

void set_memoria(int pointer, int value, struct  VM* mv){
    /// el puntero tiene 2 bytes a codigo de segmento y 2 bytes de offset
   // printf("estamos en set memoria\n");

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

void llamado_funcion(struct VM* mv, char opA, int opA_content, char opB, int opB_content, char cod_op){

    switch (cod_op) {
        case 0:{
            printf("entra a mov");
            MOV(mv, opA_content, opB_content, opA, opB);
            break;
        }

    }
}