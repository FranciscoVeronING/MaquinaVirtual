//
// Created by Fran Vergon on 3/4/2024.
//


#ifndef VIRTUAL_MACHINE_FUNCTIONS_H
#define VIRTUAL_MACHINE_FUNCTIONS_H
#include "maquina_virtual.h"

void MOV(struct VM* mv, int opA_content, int opB_content, char opA, char opB);
void ADD(struct VM* mv, int opA_content, int opB_content, char opA, char opB);
void MUL(struct VM* mv, int opA_content, int opB_content, char opA, char opB);
void SUB(struct VM* mv, int opA_content, int opB_content, char opA, char opB);
void SWAP(struct VM* mv, int opA_content, int opB_content, char opA, char opB);
void DIV(struct VM* mv, int opA_content, int opB_content, char opA, char opB, int *error);
void CMP(struct VM* mv, int opA_content, int opB_content, char opA, char opB);
void SHL(struct VM* mv, int opA_content, int opB_content, char opA, char opB);
void SHR(struct VM* mv, int opA_content, int opB_content, char opA, char opB);
void AND(struct VM* mv, int opA_content, int opB_content, char opA, char opB);
void OR(struct VM* mv, int opA_content, int opB_content, char opA, char opB);
void XOR(struct VM* mv, int opA_content, int opB_content, char opA, char opB);
void RND(struct VM* mv, int opA_content, int opB_content, char opA, char opB);
void SYS(struct VM* mv, int value);
void JMP(struct VM* mv, int opB_content, char opB, int *error);
void JZ(struct VM* mv, int opB_content, char opB, int *error);
void JP(struct VM* mv, int opB_content, char opB, int *error);
void JN(struct VM* mv, int opB_content, char opB, int *error);
void JNZ(struct VM* mv, int opB_content, char opB, int *error);
void JNP(struct VM* mv, int opB_content, char opB, int *error);
void JNN(struct VM* mv, int opB_content, char opB, int *error);
void LDL(struct VM* mv, int opB_content, char opB);
void LDH(struct VM* mv, int opB_content, char opB);
void NOT(struct VM* mv, int opB_content, char opB);
void STOP(int *error);
void set_memoria(int pointer, int value, struct  VM* mv, int cant_bytes);
int get_memoria(int pointer, struct VM mv);
void set_registro(int op, int valor, struct VM* mv);
int get_registro(int pointer, struct VM mv);
int value_op(int op_content, char op_type, struct VM mv);
int get_puntero(int opA_content,struct VM mv);
void Errores(int error);
/**
 *
 * @param functions tabla de 24 funciones
 */
void llamado_funcion(struct VM* mv, char opA,int opA_content,char opB,int opB_content,char cod_op, int *error);

#endif //VIRTUAL_MACHINE_FUNCTIONS_H
