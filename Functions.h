#ifndef VIRTUAL_MACHINE_FUNCTIONS_H
#define VIRTUAL_MACHINE_FUNCTIONS_H
#include "maquina_virtual.h"

void MOV(struct VM* mv, int opA_content, int opB_content, char opA, char opB, int *error);
void ADD(struct VM* mv, int opA_content, int opB_content, char opA, char opB, int *error);
void MUL(struct VM* mv, int opA_content, int opB_content, char opA, char opB, int *error);
void SUB(struct VM* mv, int opA_content, int opB_content, char opA, char opB, int *error);
void SWAP(struct VM* mv, int opA_content, int opB_content, char opA, char opB, int *error);
void DIV(struct VM* mv, int opA_content, int opB_content, char opA, char opB, int *error);
void CMP(struct VM* mv, int opA_content, int opB_content, char opA, char opB, int *error);
void SHL(struct VM* mv, int opA_content, int opB_content, char opA, char opB, int *error);
void SHR(struct VM* mv, int opA_content, int opB_content, char opA, char opB, int *error);
void AND(struct VM* mv, int opA_content, int opB_content, char opA, char opB, int *error);
void OR(struct VM* mv, int opA_content, int opB_content, char opA, char opB, int *error);
void XOR(struct VM* mv, int opA_content, int opB_content, char opA, char opB, int *error);
void RND(struct VM* mv, int opA_content, int opB_content, char opA, char opB, int *error);
void SYS(struct VM* mv, int value, int *error);
void JMP(struct VM* mv, int opB_content, char opB, int *error);
void JZ(struct VM* mv, int opB_content, char opB, int *error);
void JP(struct VM* mv, int opB_content, char opB, int *error);
void JN(struct VM* mv, int opB_content, char opB, int *error);
void JNZ(struct VM* mv, int opB_content, char opB, int *error);
void JNP(struct VM* mv, int opB_content, char opB, int *error);
void JNN(struct VM* mv, int opB_content, char opB, int *error);
void LDL(struct VM* mv, int opB_content, char opB, int *error);
void LDH(struct VM* mv, int opB_content, char opB, int *error);
void NOT(struct VM* mv, int opB_content, char opB, int *error);
void STOP(int *error);
void set_memoria(int pointer, unsigned int value, struct  VM* mv, int cant_bytes, int *error);
unsigned int get_memoria(int pointer, struct VM mv, int *error);
void set_registro(int op,unsigned int valor, struct VM* mv);
unsigned int get_registro(int pointer, struct VM mv);
unsigned int value_op(int op_content, char op_type, struct VM mv, int *error);
int get_puntero(int opA_content,struct VM mv);
void Errores(int error);
void llamado_funcion(struct VM* mv, char opA,int opA_content,char opB,int opB_content,char cod_op, int *error);
void show_format_write( int acum, char format, int size_cells);
void change_cc(struct  VM* mv, int value_A);
#endif //VIRTUAL_MACHINE_FUNCTIONS_H
