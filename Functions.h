//
// Created by Fran Vergon on 3/4/2024.
//

#ifndef VIRTUAL_MACHINE_FUNCTIONS_H
#define VIRTUAL_MACHINE_FUNCTIONS_H

void MOV(VM *mv, int opA_content, int opB_content, char opA, char opB);
void ADD(VM *mv, int opA_content, int opB_content, char opA, char opB);
void MUL(VM *mv, int opA_content, int opB_content, char opA, char opB);
void SUB(VM *mv, int opA_content, int opB_content, char opA, char opB);
void SWAP(VM *mv, int opA_content, int opB_content, char opA, char opB);
void DIV(VM *mv, int opA_content, int opB_content, char opA, char opB);
void CMP(VM *mv, int opA_content, int opB_content, char opA, char opB);
void SHL(VM *mv, int opA_content, int opB_content, char opA, char opB);
void SHR(VM *mv, int opA_content, int opB_content, char opA, char opB);
void AND(VM *mv, int opA_content, int opB_content, char opA, char opB);
void OR(VM *mv, int opA_content, int opB_content, char opA, char opB);
void XOR(VM *mv, int opA_content, int opB_content, char opA, char opB);
void RND(VM *mv, int opA_content, int opB_content, char opA, char opB);
void SYS(VM *mv, int opA_content, int opB_content, char opA, char opB);
void JMP(VM *mv, int opA_content, int opB_content, char opA, char opB);
void JZ(VM *mv, int opA_content, int opB_content, char opA, char opB);
void JP(VM *mv, int opA_content, int opB_content, char opA, char opB);
void JN(VM *mv, int opA_content, int opB_content, char opA, char opB);
void JNZ(VM *mv, int opA_content, int opB_content, char opA, char opB);
void JNP(VM *mv, int opA_content, int opB_content, char opA, char opB);
void JNN(VM *mv, int opA_content, int opB_content, char opA, char opB);
void LDL(VM *mv, int opA_content, int opB_content, char opA, char opB);
void LDH(VM *mv, int opA_content, int opB_content, char opA, char opB);
void NOT(VM *mv, int opA_content, int opB_content, char opA, char opB);
void STOP(VM *mv, int opA_content, int opB_content, char opA, char opB);

/**
 *
 * @param functions tabla de 24 funciones
 */
void inicia_funciones(TFunc functions[0x1F]);

#endif //VIRTUAL_MACHINE_FUNCTIONS_H
