#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Functions.h"

#ifdef _WIN32
#include <windows.h>
#endif

void MOV(struct VM* mv, int opA_content, int opB_content, char opA, char opB, int *error){
    switch (opA) {
        case 0b00: {
           // printf("entro a set memoria ");
            int pointer =  get_puntero(opA_content, *mv);
            set_memoria(pointer, value_op(opB_content, opB,*mv, error), mv, 4, error);       //memoria
            break;
        }
        case 0b10: {
            //printf("entro a set registro ");
            set_registro(opA_content, value_op(opB_content, opB, *mv, error), mv);      //registro
            break;
        }
    }
}

void ADD(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
     int value_A =(int) value_op(opA_content, opA, *mv, error);
     int value_B =(int) value_op(opB_content, opB, *mv, error);
     value_A += value_B;
    // Guardamos el valor resultante de vuelta en el operando A
    set_value(value_A, opA, opA_content, mv, error);
    //cambiamos el valor del CC(condition code
    change_cc(mv, value_A);
}

void SUB(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);
    value_A -= value_B;
    // Guardamos el valor resultante de vuelta en el operando A
    set_value(value_A, opA, opA_content, mv, error);
    //cambiamos el valor del CC(condition code
    change_cc(mv, value_A);
}

void MUL(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);
    value_A *= value_B;
    // Guardamos el valor resultante de vuelta en el operando A
    set_value(value_A, opA, opA_content, mv, error);
    //cambiamos el valor del CC(condition code
    change_cc(mv, value_A);
}

void DIV(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);

    if(value_B != 0) {
        int resto = value_A % value_B;
        value_A /= value_B;
        mv->registers_table[9] = resto;  // se guarda el valor del resto de la division entera
        // Guardamos el valor resultante de vuelta en el operando A
        set_value(value_A, opA, opA_content, mv, error);
        //cambiamos el valor del CC(condition code
        change_cc(mv, value_A);
    } else
        *error = 3;
}

void SWAP(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);
    set_value(value_B, opA, opA_content, mv, error);
    set_value(value_A, opB, opB_content, mv, error);
}

void AND(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);
    value_A &= value_B;
    // Guardamos el valor resultante de vuelta en el operando A
    set_value(value_A, opA, opA_content, mv, error);
    //cambiamos el valor del CC(condition code
    change_cc(mv, value_A);
}

void OR(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);
    value_A |= value_B;
    // Guardamos el valor resultante de vuelta en el operando A
    set_value(value_A, opA, opA_content, mv, error);
    //cambiamos el valor del CC(condition code)
    change_cc(mv, value_A);
}

void XOR(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);
    value_A ^= value_B;
    // Guardamos el valor resultante de vuelta en el operando A
    set_value(value_A, opA, opA_content, mv, error);
    //cambiamos el valor del CC(condition code
    change_cc(mv, value_A);
}

void CMP(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);
    value_A -= value_B;
    //cambiamos el valor del CC(condition code)
    change_cc(mv, value_A);
}

void SHL(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);
    // Realizamos el desplazamiento lógico a la izquierda en A por el número de posiciones indicado en B
    value_A <<= value_B;
    // Guardamos el valor resultante de vuelta en el operando A
    set_value(value_A, opA, opA_content, mv, error);
    //cambiamos el valor del CC(condition code
    change_cc(mv, value_A);
}

void SHR(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);
    // Realizamos el desplazamiento lógico a la izquierda en A por el número de posiciones indicado en B
    value_A >>= value_B;
    // Guardamos el valor resultante de vuelta en el operando A
    set_value(value_A, opA, opA_content, mv, error);
    //cambiamos el valor del CC(condition code
    change_cc(mv, value_A);
}

void SYS(struct VM* mv, int value, int *error, unsigned int *flag_break_point, char* filename_vmi) {
    char format = (char) ((*mv).registers_table[0xA] & 0x000000FF); // obtenemos el formato que esta en AL
    int cant_cells =  ((*mv).registers_table[0xC] & 0x000000FF);  // obtenemos la cantidad de celdas, esta en CL
    int size_cells =  (((*mv).registers_table[0xC] >> 8) & 0x000000FF); //obtenemos el tamaño de las celdas, esta en CH
    int aux = 0, acum;
    char aux1[100];
    int index = get_puntero(0x0D0000, *mv);
    switch (value) {
        case 1: {
            for (int n = 0; n < cant_cells; n++) {
                printf("Ingrese el valor: ");
                switch (format) {
                    case 1: {
                        scanf("%d", &aux);
                        set_memoria(index, aux, mv, size_cells, error);
                        break;
                    }
                    case 2: {
                        scanf("%s", aux1);
                        for (int i = 0; i < size_cells; i++) {
                            set_memoria(index, aux1[i], mv, 1, error);
                            index++;
                        }
                        break;
                    }
                    case 4: {
                        scanf("%o", &aux);
                        set_memoria(index, aux, mv, size_cells, error);
                        break;
                    }
                    case 8: {
                        scanf("%x", &aux);
                        set_memoria(index, aux, mv, size_cells, error);
                        break;
                    }
                }
                index+= size_cells;
            }
            break;
        }
        case 2: {    ///CASO WRITE
            /**
             * muestra en pantalla los valores contenidos a partir de la posición de memoria apuntada \n
             * por EDX, recuperando CL celdas de tamaño CH. El modo de escritura depende de la configuración \n
             *  almacenada en AL con el siguiente formato: \n
             *  %08 3 1: escribe hexadecimal \n
             *  %04 2 1: escribe octal \n
             *  %02 1 1: escribe caracteres \n
             *   %01 0 1: escribe decimal \n
             */

            // se carga en index el valor donde apunta en memoria EDX xx xx cc cc
            //  xx xx esta reservado para ubicacion del segmento
            //  cc cc esta la direccion(offset) en donde esta los datos de EDX en memoria
            index = index & 0x0000FFFF;
            for (int j = 0; j < cant_cells; j++) {
                printf("[%04X] \t", index);
                acum = 0;
                /// 0000 0000 0000 0000 0000 0000 0000 0000
                for (int k = 0; k < size_cells; k++) {
                    acum <<= 8;//se recupera cantidad de celda
                    acum |= (*mv).memory[index]; //se guarda el valor y se shiftea 1 byte a la izquierda
                    index++;
                }
                show_format_write(acum, format & 0X8, size_cells);
                show_format_write(acum, format & 0X4, size_cells);
                show_format_write(acum, format & 0X2, size_cells);
                show_format_write(acum, format & 0X1, size_cells);
                printf("\n");
            }
            break;
        }
        case 3:{
            int index = get_puntero(0x0D0000, *mv);          //obtiene la dirección de memoria del registro EDX
            int max_chars = mv->registers_table[0xC];       //obtiene la cantidad máxima de caracteres a leer desde CX
            char input;
            int count = 0;
            while ((max_chars == -1 || count < max_chars) && (input = getchar()) != '\n') {
                mv->memory[index] = input;  //almacena el char en la direccion de memoria actual
                index++;
                count++;
            }
            break;
        }
        case 4:{
          //  int index = get_puntero(0x0D0000, *mv);//obtiene la dirección de memoria del registro EDX
            while (mv->memory[index] != '\0') {
                printf("%c", mv->memory[index]);
                index++;
            }
            printf("\n");
            break;
        }
        case 7:{       //Limpia la terminal
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
            break;
        }
        case 0xf:{     //BreakPoint
            if(filename_vmi) {            //si existe archivo vmi
                modifica_vmi(mv, filename_vmi);
                char input = getchar();
                switch (input) {
                    case 'g': { // continua la ejecución
                        *flag_break_point = 0;
                        break;
                    }
                    case 'q': {// aborta la ejecución
                        *flag_break_point = 0;
                        *error = -1; // Asume que -1 es el código de error para abortar la ejecución
                        break;
                    }
                    case '\n': { // ejecuta la siguiente instrucción y vuelve a pausar
                        *flag_break_point = 1;
                        break;
                    }
                }
            }
        }
    }
}

void show_format_write( int acum, char format, int size_cells) {
    switch (format) {
        case 0x1:{
            printf("%d ", acum);
            break;
        }
        case 0x2:{
            for (int i = size_cells-1; i >= 0 ; i--) {
                int desplazamiento = 8 * i;
                int mascara = 0xFF << desplazamiento;
                int caracter = (acum & mascara) >> desplazamiento;
                if (caracter >= 32 && caracter <= 126) {
                    printf("%c", caracter);
                }
                else
                    printf(".");
            }
            printf(" ");
            break;
        }
        case 0x4:{
            printf("@%o ", acum);
            break;
        }
        case 0x8:{
            printf("%%%X ", acum);
        }
    }
}


void RND(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);
    srand(time(NULL));
    value_A =   rand()%value_B;
    // Guardamos el valor resultante de vuelta en el operando A
    set_value(value_A, opA, opA_content, mv, error);
    //cambiamos el valor del CC(condition code)
    change_cc(mv, value_A);
}

void JMP(struct VM* mv, int opA_content, char opA, int *error){
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    if(value_A < mv->segment_descriptor_table[0].size) {
        mv->registers_table[5] = (int) (value_A & 0x0000FFFF);
    }
    else
        *error = 2; //caida de segmento
}

void JZ(struct VM* mv, int opB_content, char opB, int *error){
    if( mv->registers_table[8] == 0x40000000) {
        int value_B =(int) value_op(opB_content, opB, *mv, error);
        if (value_B < mv->segment_descriptor_table[0].size){
            (*mv).registers_table[5] = value_B;
        }
        else
            *error = 2; //caida de segmento
    }
}
void JP(struct VM* mv, int opB_content, char opB, int *error){
    if(mv->registers_table[8] != 0x40000000 && mv->registers_table[8] != 0x80000000) {
        int value_B =(int) value_op(opB_content, opB, *mv, error);
        if (value_B < mv->segment_descriptor_table[0].size){
            (*mv).registers_table[5] = value_B;
        }
        else
            *error = 2; //caida de segmento
    }
}

void JN(struct VM* mv, int opB_content, char opB, int *error){
    if(mv->registers_table[8] == 0x80000000) {
        int value_B =(int) value_op(opB_content, opB, *mv, error);
        if (value_B < mv->segment_descriptor_table[0].size){
            (*mv).registers_table[5] = value_B;
        }
        else
            *error = 2; //caida de segmento
    }
}
void JNZ(struct VM* mv, int opB_content, char opB, int *error){
    if(mv->registers_table[8] == 0x80000000 || mv->registers_table[8] == 0x00000000) {
        int value_B =(int) value_op(opB_content, opB, *mv, error);
        if (value_B < mv->segment_descriptor_table[0].size){
            (*mv).registers_table[5] = value_B;
        }
        else
            *error = 2; //caida de segmento
    }
}

void JNP(struct VM* mv, int opB_content, char opB, int *error){
    if( mv->registers_table[8] == 0x40000000 || mv->registers_table[8] == 0x80000000) {
        int value_B =(int) value_op(opB_content, opB, *mv, error);
        if (value_B < mv->segment_descriptor_table[0].size){
            (*mv).registers_table[5] = value_B;
        }
        else
            *error = 2; //caida de segmento
    }
}

void JNN(struct VM* mv, int opB_content, char opB, int *error){
    if(mv->registers_table[8] == 0x40000000 || mv->registers_table[8] == 0x00000000) {
        int value_B =(int) value_op(opB_content, opB, *mv, error);
        if (value_B < mv->segment_descriptor_table[0].size){
            (*mv).registers_table[5] = value_B;
        }
        else
            *error = 2; //caida de segmento
    }
}

void LDL(struct VM* mv, int opA_content, char opA, int *error){
    int value = (int)value_op(opA_content, opA, *mv, error);
    mv->registers_table[9] |= value  & 0x0000FFFF;

}
void LDH(struct VM* mv, int opA_content, char opA, int *error){
    int value = (int)value_op(opA_content, opA, *mv, error);
    mv->registers_table[9] |= value  << 16;
}
void NOT(struct VM* mv, int opA_content, char opA, int *error){
    int value = (int)value_op(opA_content, opA, *mv, error);
    value = ~value;
    set_value(value, opA, opA_content, mv, error);
    change_cc(mv, value);
}

void STOP(int *error){
   *error =  -1;
}

void PUSH(struct VM* mv, int opA_content, char opA, int *error) {
    int sp_reg = get_registro(0x6, *mv);
    int ss_reg = get_registro(0x3, *mv);
    sp_reg -= 4;
    if (sp_reg < ss_reg)
        *error = 5;//stack Overflow
    else {
        set_registro(0x6, sp_reg, mv);
        int value = value_op(opA_content, opA, *mv, error);
        //set_memoria(sp_reg, value, mv, 4, error);
        int index = sp_reg & 0x0000FFFF;//solo ponemos como index el offset delpuntero, en getpuntero hicimos la suma de os 2 offsets
        int aux = 3;
        int index_sdt = (int)(sp_reg  >> 16);
        index += (*mv).segment_descriptor_table[index_sdt].base;
        if((index >= (*mv).segment_descriptor_table[index_sdt].base) && (index <= (((*mv).segment_descriptor_table[index_sdt].base + (*mv).segment_descriptor_table[index_sdt].size) ))) {
            for (int i = 0; i < 4; ++i) {
                (*mv).memory[index] = (char) (value >> (8 * aux));
                aux--;
                index++;
            }
        }
        else
            *error = 2;
    }
}

void POP(struct VM* mv, int opA_content, char opA, int *error){
    int sp_reg = get_registro(0x6,*mv);
    int ss_reg = get_registro(0x3,*mv);
    if(sp_reg > ss_reg + mv->segment_descriptor_table[mv->registers_table[3]>>16].size )
        *error = 6; // Stack Underflow
    else{
        switch (opA) {
            case 0 :{

                //set_memoria(get_puntero(opA_content,(*mv)), value_op(0x60000, 0,*mv, error), mv, 4, error);
                int value = value_op(0x60000, 0,*mv, error);
                int index = sp_reg & 0x0000FFFF;//solo ponemos como index el offset delpuntero, en getpuntero hicimos la suma de os 2 offsets
                int aux = 3;
                int index_sdt = (int)(sp_reg  >> 16);
                index += (*mv).segment_descriptor_table[index_sdt].base;
                if((index >= (*mv).segment_descriptor_table[index_sdt].base) && (index <= (((*mv).segment_descriptor_table[index_sdt].base + (*mv).segment_descriptor_table[index_sdt].size)))) {
                    for (int i = 0; i < 4; ++i) {
                        (*mv).memory[index] = (char) (value >> (8 * aux));
                        aux--;
                        index++;
                    }
                }
                else
                    *error = 2;
                break;
            }
            case 2 :{
                set_registro(opA_content, value_op(0x60000, 0, *mv, error), mv);
                break;
            }
        }
        sp_reg += 4;
        set_registro(0x6,sp_reg,mv);
        }
}

void CALL(struct VM* mv, int opB_content, char opB, int *error){
    // pushea el valor actual de IP en la pila
    int sp_reg = get_registro(0x6, *mv);
    int ss_reg = get_registro(0x3, *mv);
    sp_reg -= 4;
    if (sp_reg < ss_reg)
        *error = 5; //stack Overflow
    else {
        set_registro(0x6, sp_reg, mv);
        int value = (*mv).registers_table[5] & 0x0000ffff;
        //set_memoria(sp_reg, value, mv, 4, error);
        int index = sp_reg & 0x0000FFFF;//solo ponemos como index el offset delpuntero, en getpuntero hicimos la suma de os 2 offsets
        int aux = 3;
        int index_sdt = (int)(sp_reg  >> 16);
        index += (*mv).segment_descriptor_table[index_sdt].base;
        if((index >= (*mv).segment_descriptor_table[index_sdt].base) && (index <= (((*mv).segment_descriptor_table[index_sdt].base + (*mv).segment_descriptor_table[index_sdt].size) ))) {
            for (int i = 0; i < 4; ++i) {
                (*mv).memory[index] = (char) (value >> (8 * aux));
                aux--;
                index++;
            }
        }
        else
            *error = 2;
    }
    // salta a la direccion dada por el operando
    JMP(mv, opB_content, opB, error);
}

void RET(struct VM *mv, int *error){
    int sp_reg = get_registro(0x6,*mv);
    int ss_reg = get_registro(0x3,*mv);
    if(sp_reg > ss_reg + mv->segment_descriptor_table[mv->registers_table[3]>>16].size )
        *error = 6; // Stack Underflow
    else {
        // Realiza un salto a la dirección de memoria extraída
        mv->registers_table[5] = get_memoria(get_puntero(0x60000,(*mv)),(*mv),error,0);
        sp_reg += 4;
        set_registro(0x6, sp_reg, mv);
    }
}

int get_puntero(int op_content, struct VM mv){
  int pointer;
  char index_register = (char)((op_content >> 16) & 0xf);
  int index = (int)((mv.registers_table[index_register] >> 16)& 0xff);
  int base = mv.segment_descriptor_table[index].base;
  pointer = (index << 16) + base + (op_content & 0x0000FFFF) + (mv.registers_table[index_register] & 0x0000ffff);
  return pointer;
}


void set_memoria(int pointer, unsigned int value, struct  VM* mv, int cant_bytes, int * error){
    /// el puntero tiene 2 bytes a codigo de segmento y 2 bytes de offset
    int index = pointer & 0x0000FFFF;//solo ponemos como index el offset delpuntero, en getpuntero hicimos la suma de os 2 offsets
    int aux = cant_bytes - 1;
    int index_sdt = (int)(pointer  >> 16);
    //index += (*mv).segment_descriptor_table[index_sdt].base;
    if((index >= (*mv).segment_descriptor_table[index_sdt].base) && (index <= (((*mv).segment_descriptor_table[index_sdt].base + (*mv).segment_descriptor_table[index_sdt].size) - 4))) {
        for (int i = 0; i < cant_bytes; ++i) {
            (*mv).memory[index] = (char) (value >> (8 * aux));
            aux--;
            index++;
        }
    }
    else
        *error = 2;
}

unsigned int get_memoria(int pointer, struct VM mv, int *error, int type){
    ///hay 2 opciones, si es memoria directa, o si es la memoria de un registro
    unsigned int value = 0;
    int index = pointer & 0x0000FFFF; //OFFSET
    int index_sdt = (int)(pointer  >> 16); //OFFSET
    //index += mv.segment_descriptor_table[index_sdt].base;
    if((index >= mv.segment_descriptor_table[index_sdt].base) && (index <= ((mv.segment_descriptor_table[index_sdt].base + mv.segment_descriptor_table[index_sdt].size) - 4))) {
         switch (type) {
            case 3:{ ///byte
                value = (mv).memory[index+3];
                break;
            }
            case 2:{ ///word
                value = (mv).memory[index] << 8 | (mv).memory[index + 1]; ///CARGAMOS LOS 2 BYTES MAS SIGNIFICATIVOS
                break;
            }
            case 0:{
                value = (mv).memory[index] << 24 | (mv).memory[index + 1] << 16 | (mv).memory[index + 2 ] << 8 | (mv).memory[index + 3] ;
                break;
            }
        }
    }
    else{
        *error = 2;
    }
    //printf("\n %x value este es el valor en getmemoria\n", value);
    return value;
}

unsigned int value_op(int op_content, char op_type, struct VM mv, int *error){  //obtiene el valor del operando
    unsigned int value;
    switch(op_type){
        case 0: {   //caso de memoria
            /// 0000 xxxx 11111111 11111111
            int type = (op_content >> 22) & 0x00000003;
            op_content = op_content & 0x000FFFFF;
            int pointer = get_puntero(op_content,mv);
            value = get_memoria(pointer, mv, error, type);
            break;
        }
        case 1: { //caso inmediato
            if((op_content & 0x8000) == 0x8000)
                op_content = (int)(0xFFFF0000 | op_content);
            value = op_content;
            break;
        }
        case 2: { //caso registro
            value = get_registro(op_content, mv);
            break;
        }
    }
    return value;
}


unsigned int get_registro(int op, struct VM mv) {
    int cod_reg, sec_reg, valor;
    sec_reg = (op >> 4);        //almacena el tipo de registro
    cod_reg = op & 0x0F;             //almacena el registro
    switch (sec_reg) {
        case 0:{
            valor = mv.registers_table[cod_reg];
            break;
        }
        case 1:{
            char temp = mv.registers_table[cod_reg] & 0xFF;
            valor = (temp & 0x80) ? (int)(temp | 0xFFFFFF00) : (int)temp;
            break;
        }
        case 2:{
            char temp = (mv.registers_table[cod_reg] & 0xFF00)>>8;
            valor = (temp & 0x80) ? (int)(temp | 0xFFFFFF00) : (int)temp;
            break;
        }
        case 3:{
            unsigned short int temp = mv.registers_table[cod_reg] & 0xFFFF;
            valor = (temp & 0x8000) ? (int)(temp | 0xFFFFFF00) : (int)temp;
            break;
        }
    }
    return valor;
}

void set_registro(int op,unsigned int valor, struct VM* mv){
    //cambia el valor de un registro
    int cod_reg, sec_reg;
    sec_reg = (op >> 4);        //almacena el tipo de registro
    cod_reg = op & 0xF;             //almacena el registro
    switch (sec_reg) {
        case 0:{
            (*mv).registers_table[cod_reg] = (int)valor; //Caso EAX
            break;
        }
        case 1:{
            (*mv).registers_table[cod_reg] = (int)((*mv).registers_table[cod_reg] & 0xFFFFFF00) + (valor & 0x000000ff); //se quieren mantener los 24 bits y modificar los ultimos 8 (caso AL)
            break;
        }
        case 2:{
            (*mv).registers_table[cod_reg] = (int)((*mv).registers_table[cod_reg] & 0xFFFF00FF) | ((valor & 0x000000ff) << 8); // Caso modificar AH
            break;
        }
        case 3:{
            (*mv).registers_table[cod_reg] = (int)((*mv).registers_table[cod_reg] & 0xFFFF0000) + (valor & 0x0000FFFF); //Caso modificar AX
            break;
        }
    }
}

void set_value(int value, char op, int op_content, struct VM *mv, int *error) {
    if(op == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(op_content, *mv);
        set_memoria(pointer, value, mv, 4, error);
    }
    else if(op == 2)  // Si el operando A es un registro
        set_registro(op_content,value, mv);
}

void llamado_funcion(struct VM* mv, char opA, int opA_content, char opB, int opB_content, char cod_op, int *error, unsigned int *flag_break_point, char* filename_vmi){
    switch (cod_op) {
        case 0:{
            //printf("entra a mov");
            MOV(mv, opA_content, opB_content, opA, opB, error);
            break;
        }
        case 1:{
            //printf("entra a ADD");
            ADD(mv,opA_content,opB_content,opA,opB, error);
            break;
        }
        case 2:{
           // printf("entra a SUB");
            SUB(mv,opA_content,opB_content,opA,opB, error);
            break;
        }
        case 3:{
            //printf("entra a SWAP");
            SWAP(mv,opA_content,opB_content,opA,opB, error);
            break;
        }
        case 4:{
            //printf("entra a MUL");
            MUL(mv,opA_content,opB_content,opA,opB, error);
            break;
        }
        case 5:{
            //printf("entra a DIV");
            DIV(mv,opA_content,opB_content,opA,opB, error);
            break;
        }
        case 6:{
          //  printf("entra a CMP");
            CMP(mv,opA_content,opB_content,opA,opB, error);
            break;
        }
        case 7:{
            //printf("entra a SHL");
            SHL(mv,opA_content,opB_content,opA,opB, error);
            break;
        }
        case 8:{
           //printf("entra a SHR");
            SHR(mv,opA_content,opB_content,opA,opB, error);
            break;
        }
        case 9:{
            //printf("entra a AND");
            AND(mv,opA_content,opB_content,opA,opB, error);
            break;
        }
        case 0xA:{
            //printf("entra a OR");
            OR(mv,opA_content,opB_content,opA,opB, error);
            break;
        }
        case 0xB:{
            //printf("entra a XOR");
            XOR(mv,opA_content,opB_content,opA,opB, error);
            break;
        }
        case 0xC:{
            //printf("entra a RND");
            RND(mv,opA_content,opB_content,opA,opB, error);
            break;
        }
        case 0x10:{
           // printf("entra a SYS");
            SYS(mv, opB_content, error, flag_break_point, filename_vmi);
            break;
        }
        case 0x11:{
           // printf("entra a JMP");
            JMP(mv,opB_content, opB, error);
            break;
        }
        case 0x12:{
           // printf("entra a JZ");
            JZ(mv,opB_content, opB, error);
            break;
        }
        case 0x13:{
            //printf("entra a JP");
            JP(mv,opB_content, opB, error);
            break;
        }
        case 0x14:{
            //printf("entra a JN");
            JN(mv,opB_content, opB, error);
            break;
        }
        case 0x15:{
            //printf("entra a JNZ");
            JNZ(mv,opB_content, opB, error);
            break;
        }
        case 0x16:{
            //printf("entra a JNP");
            JNP(mv,opB_content, opB, error);
            break;
        }
        case 0x17:{
            //printf("entra a JNN");
            JNN(mv,opB_content, opB, error);
            break;
        }
        case 0x18:{
            //printf("entra a LDL");
            LDL(mv,opB_content, opB, error);
            break;
        }
        case 0x19:{
            //printf("entra a LDH");
            LDH(mv,opB_content, opB, error);
            break;
        }
        case 0x1A:{
            //printf("entra a NOT");
            NOT(mv,opB_content, opB, error);
            break;
        }
        case 0x1B:{
            //printf("entra a PUSH");
            PUSH(mv,opB_content, opB, error);
            break;
        }
        case 0x1C:{
            //printf("entra a POP");
            POP(mv,opB_content, opB, error);
            break;
        }
        case 0x1D:{
            //printf("entra a CALL");
            CALL(mv, opB_content, opB, error);
            break;
        }
        case 0x1E:{
            //printf("entra a RET");
            RET(mv, error);
            break;
        }
        case 0x1F:{
            // printf("entra a STOP");
            STOP(error);
            break;
        }
        default: {
            *error = 1;
            break;
        }
    }
}
void Errores(int error){
    /**
    * error = 1 Instruccion inválida
    * error = 2 Caida de segmento
    * error = 3 Division por Cero
    * error = -1 flag del STOP
    * error = 4 Memoria insuficiente
    * error = 5 Stack Overflow
    * error = 6 Stack Underflow
    */
    switch(error){
        case 1: {
            printf("\nError: Instruccion invalida");
            break;
        }
        case 2: {
            printf("\nError: Caida de segmento");
            break;
        }
        case 3: {
            printf("\nError: Division por Cero");
            break;
        }
        case 4: {
            printf("\nError: Memoria insuficiente");
            break;
        }
        case 5: {
            printf("\nError: Stack Overflow");
            break;
        }
        case 6: {
            printf("\nError: Stack Underflow");
            break;
        }
    }
}

void change_cc(struct  VM* mv, int value_A){
    if(value_A < 0)
        (*mv).registers_table[8] = (int)0x80000000;
    else
    if(value_A == 0)
        (*mv).registers_table[8] = 0x40000000;
    else
        (*mv).registers_table[8] = 0x00000000;
}

void modifica_vmi(struct VM* mv, char* filename_vmi){
    FILE *file_mv_vmi;
    char header[8];
    unsigned int auxSizeMemory = mv->size_memory/1024;
    char char1 = (char)(auxSizeMemory & 0xFF);
    char char2 = (char)((auxSizeMemory >> 8) & 0xFF);
    file_mv_vmi = fopen(filename_vmi, "wb");
    if (file_mv_vmi == NULL)
        perror("Error al abrir el archivo .vmi \n");
    else {
        strcpy(header, "VMX241");
        strcat(header, &char1);
        strcat(header, &char2);
        fwrite(header, sizeof(int), 16, file_mv_vmi);
        // REGISTROS
        fwrite(mv->registers_table, sizeof(int), 16, file_mv_vmi);
        // SDT
        for (int i = 0; i < 8; ++i) {
            fwrite(&(mv->segment_descriptor_table[i].base), sizeof(unsigned short int), 1, file_mv_vmi);
            fwrite(&(mv->segment_descriptor_table[i].size), sizeof(unsigned short int), 1, file_mv_vmi);
        }
        // MEMORIA
        fwrite(mv->memory, sizeof(char), mv->size_memory, file_mv_vmi);
    }
    fclose(file_mv_vmi);
}