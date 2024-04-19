#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Functions.h"

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
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv, 4, error);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA_content,value_A, mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = (int)0x80000000;
    else
        if(value_A == 0)
            mv->registers_table[8] = 0x40000000;
        else
            mv->registers_table[8] = 0x00000000;
}

void SUB(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);

    value_A -= value_B;

    // Guardamos el valor resultante de vuelta en el operando A
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv, 4, error);
    }
    else
        if(opA == 2)  // Si el operando A es un registro
            set_registro(opA_content,value_A,mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = (int)0x80000000;
    else
        if(value_A == 0)
            mv->registers_table[8] = 0x40000000;
        else
            mv->registers_table[8] = 0x00000000;
}

void MUL(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);

    value_A *= value_B;

    // Guardamos el valor resultante de vuelta en el operando A
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv, 4, error);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA_content,value_A,mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = (int)0x80000000;
    else
        if(value_A == 0)
            mv->registers_table[8] = 0x40000000;
        else
            mv->registers_table[8] = 0x00000000;
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
        if (opA == 0) { // Si el operando A es de memoria
            int pointer = get_puntero(opA_content, *mv);
            set_memoria(pointer, value_A, mv, 4, error);
        } else if (opA == 2)  // Si el operando A es un registro
            set_registro(opA_content, value_A, mv);

        //cambiamos el valor del CC(condition code
        if(value_A < 0)
            mv->registers_table[8] = (int)0x80000000;
        else
            if(value_A == 0)
                mv->registers_table[8] = 0x40000000;
            else
                mv->registers_table[8] = 0x00000000;
    } else
        *error = 3;


}

void SWAP(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);
    int pointer;
    if(opA == 0) { // Si el operando A es de memoria
        pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_B, mv, 4, error);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA_content,value_B, mv);
    if(opB == 0) { // Si el operando B es de memoria
        pointer = get_puntero(opB_content, *mv);
        set_memoria(pointer, value_A, mv, 4, error);
    }
    else if(opB == 2)  // Si el operando B es un registro
        set_registro(opB_content,value_A, mv);
}

void AND(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);

    value_A &= value_B;

    // Guardamos el valor resultante de vuelta en el operando A
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv, 4, error);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA_content,value_A,mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = (int)0x80000000;
    else
        if(value_A == 0)
            mv->registers_table[8] = 0x40000000;
        else
            mv->registers_table[8] = 0x00000000;
}

void OR(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);

    value_A |= value_B;

    // Guardamos el valor resultante de vuelta en el operando A
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv, 4, error);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA_content,value_A,mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = (int)0x80000000;
    else
        if(value_A == 0)
            mv->registers_table[8] = 0x40000000;
        else
            mv->registers_table[8] = 0x00000000;
}

void XOR(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);

    value_A ^= value_B;

    // Guardamos el valor resultante de vuelta en el operando A
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv, 4, error);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA_content,value_A,mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = (int)0x80000000;
    else
        if(value_A == 0)
            mv->registers_table[8] = 0x40000000;
        else
            mv->registers_table[8] = 0x00000000;
}

void CMP(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);
    printf("valor A %X", value_A);
    printf("valor B %X", value_B);

    value_A -= value_B;
    //cambiamos el valor del CC(condition code)
    if(value_A < 0)
        mv->registers_table[8] = (int)0x80000000;
    else
        if(value_A == 0)
            mv->registers_table[8] = 0x40000000;
        else
            mv->registers_table[8] = 0x00000000;
}

void SHL(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);

    // Realizamos el desplazamiento lógico a la izquierda en A por el número de posiciones indicado en B
    value_A <<= value_B;

    // Guardamos el valor resultante de vuelta en el operando A
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv, 4, error);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA_content,value_A,mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = (int)0x80000000;
    else
        if(value_A == 0)
            mv->registers_table[8] = 0x40000000;
        else
            mv->registers_table[8] = 0x00000000;
}

void SHR(struct VM* mv, int opA_content, int opB_content , char opA, char opB, int *error) {
    // Obtenemos el valor del operando A y B
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    int value_B =(int) value_op(opB_content, opB, *mv, error);

    // Realizamos el desplazamiento lógico a la izquierda en A por el número de posiciones indicado en B
    value_A >>= value_B;

    // Guardamos el valor resultante de vuelta en el operando A
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv, 4, error);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA_content,value_A,mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = (int)0x80000000;
    else
        if(value_A == 0)
            mv->registers_table[8] = 0x40000000;
        else
            mv->registers_table[8] = 0x00000000;
}

void SYS(struct VM* mv, int value,int *error){
    char format = (char)((*mv).registers_table[0xA] & 0x000000FF); // obtenemos el formato que esta en AL
    char cant_cells = (char)((*mv).registers_table[0xC] & 0x000000FF);  // obtenemos la cantidad de celdas, esta en CL
    char size_cells = (char)(((*mv).registers_table[0xC] >> 8) & 0x000000FF); //obtenemos el tamaño de las celdas, esta en CH
    int aux=0;
    int  acum;
    char aux1[100];
    char pos_sdt = (char) ((*mv).registers_table[0xD] >> 16);
    //int index =(*mv).segment_descriptor_table[pos_sdt].base + ((*mv).registers_table[0xD] & 0x0000FFFF);
    int index = get_puntero(0x0D0000, *mv);
    printf("    ESTE ES INDEX :%X ", index);
    switch (value) {
        case 1:{
            printf("Ingrese el valor");
            switch(format){
                case 1:{
                    scanf("%d", &aux);
                    set_memoria(index, aux, mv, size_cells, error);

                    break;
                }
                case 2:{
                    scanf("%s", aux1);
                    for(int i=0; i<cant_cells; i++){
                        set_memoria(index, aux1[i], mv, 1, error);
                        index++;
                    }
                    break;
                }
                case 4:{
                    scanf("%o", &aux);
                    set_memoria(index, aux, mv, size_cells, error);
                    break;
                }
                case 8:{
                    scanf("%x", &aux);
                    set_memoria(index, aux, mv, size_cells, error);
                    break;
                }
            }
            break;
        }
        case 2:{    ///CASO WRITE
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
                for (int k = 0; k < size_cells; k++) {
                    acum <<= 8;//se recupera cantidad de celda
                    acum |= (*mv).memory[index]; //se guarda el valor y se shiftea 1 byte a la izquierda
                    index++;
                }
                show_format_write(acum, format & 0X1, size_cells);
                show_format_write(acum, format & 0X2, size_cells);
                show_format_write(acum, format & 0X4, size_cells);
                show_format_write(acum, format & 0X8, size_cells);
                printf("\n");
            }
            break;
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
            for (int i = 0; i < size_cells ; ++i) {
                int desplazamiento = 8 * i;
                int mascara = 0xFF << desplazamiento;
                int caracter = (acum & mascara) >> desplazamiento;
                if ((caracter >= 32 && caracter <= 126) || (caracter >= 160 && caracter <= 255)) {
                    printf("%c ", caracter);
                }
                else
                    printf(". ");
            }
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
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value_A, mv, 4, error);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA_content,value_A,mv);

    //cambiamos el valor del CC(condition code
    if(value_A < 0)
        mv->registers_table[8] = (int)0x80000000;
    else
        if(value_A == 0)
            mv->registers_table[8] = 0x40000000;
        else
            mv->registers_table[8] = 0x00000000;
}


void JMP(struct VM* mv, int opA_content, char opA, int *error){
    int value_A =(int) value_op(opA_content, opA, *mv, error);
    if(value_A < mv->segment_descriptor_table[0].size) {
        mv->registers_table[5] = (int) (value_A & 0x0000FFFF);
       // *flag = 1;
    }
    else
        *error = 2; //caida de segmento
}

void JZ(struct VM* mv, int opB_content, char opB, int *error){
    int aux = (mv->registers_table[8] >> 28) & 0x0000000F;
    if( mv->registers_table[8] == 0x40000000) {
        int value_B =(int) value_op(opB_content, opB, *mv, error);
        if (value_B < mv->segment_descriptor_table[0].size){
            (*mv).registers_table[5] = value_B;
           // *flag = 1;
        }
        else
            *error = 2; //caida de segmento
    }
}
void JP(struct VM* mv, int opB_content, char opB, int *error){
    int aux = (mv->registers_table[8] >> 28) & 0x0000000F;
    if((char) aux == 0x0) {
        int value_B =(int) value_op(opB_content, opB, *mv, error);
        if (value_B < mv->segment_descriptor_table[0].size){
            (*mv).registers_table[5] = value_B;
           // *flag = 1;
        }
        else
            *error = 2; //caida de segmento
    }
}

void JN(struct VM* mv, int opB_content, char opB, int *error){
    int aux = (mv->registers_table[8] >> 28) & 0x0000000F;
    if( aux == 0x8) {
        int value_B =(int) value_op(opB_content, opB, *mv, error);
        if (value_B < mv->segment_descriptor_table[0].size){
            (*mv).registers_table[5] = value_B;
          //  *flag = 1;
        }
        else
            *error = 2; //caida de segmento
    }
}
void JNZ(struct VM* mv, int opB_content, char opB, int *error){
    int aux = (mv->registers_table[8] >> 28) & 0x0000000F;
    if( aux == 0x8 || aux == 0x0) {
        int value_B =(int) value_op(opB_content, opB, *mv, error);
        if (value_B < mv->segment_descriptor_table[0].size){
            (*mv).registers_table[5] = value_B;
            //*flag = 1;
        }
        else
            *error = 2; //caida de segmento
    }
}

void JNP(struct VM* mv, int opB_content, char opB, int *error){
    int aux = (mv->registers_table[8] >> 28) & 0x0000000F;
    if( aux == 0x4 || aux == 0x8) {
        int value_B =(int) value_op(opB_content, opB, *mv, error);
        if (value_B < mv->segment_descriptor_table[0].size){
            (*mv).registers_table[5] = value_B;
            //*flag = 1;
        }
        else
            *error = 2; //caida de segmento
    }
}

void JNN(struct VM* mv, int opB_content, char opB, int *error){
    int aux = (mv->registers_table[8] >> 28) & 0x0000000F;
    if( aux == 0x4 || aux == 0x0) {
        int value_B =(int) value_op(opB_content, opB, *mv, error);
        if (value_B < mv->segment_descriptor_table[0].size){
            (*mv).registers_table[5] = value_B;
           // *flag = 1;
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
    if(opA == 0) { // Si el operando A es de memoria
        int pointer = get_puntero(opA_content, *mv);
        set_memoria(pointer, value, mv, 4, error);
    }
    else if(opA == 2)  // Si el operando A es un registro
        set_registro(opA_content,value, mv);

    if(value < 0)
        mv->registers_table[8] = (int)0x80000000;
    else if(value == 0)
        mv->registers_table[8] = 0x40000000;
    else
        mv->registers_table[8] = 0x00000000;

}

void STOP(int *error){
   *error =  -1;
};

int get_puntero(int op_content,struct VM mv){
    int pointer;
    char index = op_content >> 16;
    int aux = mv.registers_table[index] >> 16;
    pointer = 0x00010000 | mv.registers_table[index]; //se asigna el contenido que haya en el puntero, si es DS -> 0x00010000
   if(index == 1 || aux == 1)
       pointer |= mv.segment_descriptor_table[aux].base; //
   pointer += (op_content & 0x0000FFFF) ;
    //printf(" \n %X este es pointer\n", pointer);

    return pointer;
}

void set_memoria(int pointer, unsigned int value, struct  VM* mv, int cant_bytes, int * error){
    /// el puntero tiene 2 bytes a codigo de segmento y 2 bytes de offset
   /* printf("estamos en set memoria\n");
    printf("%08X  puntero\n",pointer);
    printf("%04X  valor\n", value);*/
    int index = pointer & 0x0000FFFF;//solo ponemos como index el offset delpuntero, en getpuntero hicimos la suma de os 2 offsets
    int aux = cant_bytes - 1;
    int index_sdt = (int)(pointer  >> 16);
  //  printf("este es el comienzo de DS PA %x   y ", (*mv).segment_descriptor_table[index_sdt].base);
    printf("este es el index PA %x", index);  //81
    printf( "\n%X", (*mv).segment_descriptor_table[index_sdt].base );  //0
    printf( "\n%X", ((*mv).segment_descriptor_table[index_sdt].size - 4) );  //7D
    if((index >= (*mv).segment_descriptor_table[index_sdt].base) && (index <= ((*mv).segment_descriptor_table[index_sdt].size - 4))) {
        for (int i = 0; i < cant_bytes; ++i) {
            (*mv).memory[index] = (char) (value >> (8 * aux));
            aux--;
            index++;
        }
        index--;
    }
    else
        *error = 2;
}

unsigned int get_memoria(int pointer, struct VM mv, int *error){
    ///hay 2 opciones, si es memoria directa, o si es la memoria de un registro
  // printf("\n entro a get memoria \n");
    unsigned int value = 0;
    int index = pointer & 0x0000FFFF; //OFFSET
    int index_sdt = (int)(pointer & 0xFFFF0000)>>16;
    if((index >= mv.segment_descriptor_table[index_sdt].base) && (index <= (mv.segment_descriptor_table[index_sdt].size - 4))){
        value = (mv).memory[index] << 24 | (mv).memory[index + 1] << 16 | (mv).memory[index + 2 ] << 8 |(mv).memory[index + 3] ;
    }
    else{
        *error = 2;
    }
   // printf("\n %x value este es el valor en getmemoria\n", value);
    return value;
}



unsigned int value_op(int op_content, char op_type, struct VM mv, int *error){  //obtiene el valor del operando
    unsigned int value;
    switch(op_type){
        case 0: {   //caso de memoria
            /// 0000 xxxx 11111111 11111111
            int pointer = get_puntero(op_content,mv);
           value = get_memoria(pointer, mv, error);
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
unsigned int get_registro(int op, struct VM mv){ //obtiene el valor de un registro
    int cod_reg, sec_reg;
    unsigned int valor;
    sec_reg = (op >> 4) & 0x3;        //almacena el tipo de registro
    cod_reg = op & 0x0F;             //almacena el registro
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
            valor = (mv.registers_table[cod_reg] & 0xFF00)>>8;
            break;
        }
        case 3:{
            valor = mv.registers_table[cod_reg] & 0xFFFF;
            break;
        }
    }
   // printf("\n valor a asignar EN REGISTRO 111 %08X", valor);
    return valor;
}

void set_registro(int op,unsigned int valor, struct VM* mv){
    //cambia el valor de un registro
    int cod_reg, sec_reg;
    sec_reg = (op >> 4) & 0x3;        //almacena el tipo de registro
    cod_reg = op & 0xF;             //almacena el registro
    switch (sec_reg) {
        case 0:{
           //printf("\n valor a asignar EN REGISTRO %08X", valor);
            if((valor & 0x8000) == 0x8000){
                valor = (int)0xFFFF0000 + valor;
                (*mv).registers_table[cod_reg] = (int)valor;
            }
            else
                (*mv).registers_table[cod_reg] = (int)valor; //Caso EAX
            break;
        }
        case 1:{
            (*mv).registers_table[cod_reg] = (int)((*mv).registers_table[cod_reg] & 0xFFFFFF00) | valor; //se quieren mantener los 24 bits y modificar los ultimos 8 (caso AL)
            break;
        }
        case 2:{
            (*mv).registers_table[cod_reg] = (int)((*mv).registers_table[cod_reg] & 0xFFFF00FF) | (valor << 8); // Caso modificar AH
            break;
        }
        case 3:{
            (*mv).registers_table[cod_reg] = (int)((*mv).registers_table[cod_reg] & 0xFFFF0000) | valor; //Caso modificar AX
            break;
        }
    }
}

void llamado_funcion(struct VM* mv, char opA, int opA_content, char opB, int opB_content, char cod_op, int *error){
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
            SYS(mv, opB_content, error);
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
    }
}