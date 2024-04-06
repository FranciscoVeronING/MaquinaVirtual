#include "Functions.h"

void MOV(struct VM *mv, int opA_content, int opB_content, char opA, char opB){
    switch (opB) {
        case 0b00:  set_memoria(opA, opB, opA_content, opB_content, mv);       //memoria
            break;
        case 0b10:  setregistro(opA, opB, opA_content, opB_content, mv);      //registro
            break;
    }
}


