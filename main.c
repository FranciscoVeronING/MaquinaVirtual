#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maquina_virtual.h"
#include "Functions.c"
#include "maquina_virtual.c"

void carga_regs(char regs_tags[0x40][4]);
void carga_nemonics(char nemonicos_tags[0x20][5]);

int get_registro(int op, struct VM mv);
void set_registro(int op, int valor, struct VM* mv);
void dissasembler_func(struct  VM mv);

void set_registers_table(struct VM *pVm);

int main(int argc, char *argv[]) {
/// arg[0] = .exe; arg([1] .vmx
    // Abrir el archivo en modo de lectura binaria
    FILE *file_mv;
    int dissasembler = 0;
    struct VM mv;

    if(argv[2] && strcmp(argv[2],"-d") == 0)
        dissasembler = 1;

    file_mv = fopen(argv[1], "rb");
    if (file_mv == NULL) {
        perror("Error al abrir el archivo ");
        return 1;
    }
    char header[5];

    // Leer los primeros 6 bytes del archivo
    fread(&header,sizeof(char),8,file_mv);
    //strcat(header,"\0");
    ///VMX241
    printf("%s",header);
    //Valida que la cabecera del archivo esta bien
    if(strncmp(header,"VMX24",5) != 0){// O PONER header != 0xVMX241
        perror("Error al abrir el archivo , header erroneo");
        fclose(file_mv);
        return 1;
    }
    else{
        //COMIENZO DE CARGA DE ARCHIVO EN CS (Code Segment)
        //Creacion de tabla de descriptores de segmentos
        //carga de CS e inicializacion de SDT

        ///load_memory(file_mv,mv);
        unsigned int size_cs;
        size_cs = (header[6] << 8 ) | header[7];
        printf(" %d", size_cs);
        int i = 0;
        unsigned char aux;
        fread(&aux, sizeof( char), 1, file_mv);
        while(!feof(file_mv)){
            //carga la memoria ram con el codigo de segmento

            mv.memory[i] = aux;
            i++;
            fread(&aux, sizeof( char), 1, file_mv);
        }
        fclose(file_mv);

        scanf("%c",&aux);

        //carga de Tabla de descriptores de segmento
        mv.segment_descriptor_table[0].base = 0x0000;
        mv.segment_descriptor_table[0].size = size_cs;
        mv.segment_descriptor_table[1].base = size_cs;
        mv.segment_descriptor_table[1].size = MEMORY_SIZE - size_cs;
        //Se inicializa tabla de registros
        set_registers_table(&mv);
       // printf("%x DS\n", mv.registers_table[1]);
       ///EJECUCION

        int ip = mv.segment_descriptor_table[0].base;
        int j = 0, opB_content, opA_content;
        char pos_act;
        char opA, opB, cod_op;
        char opA_size, opB_size;
        /**
     * error = 1 Instruccion inválida
     * error = 2 Caida de segmento
     * error = 3 Division por Cero
     * error = -1 flag del STOP
     */
        int error = 0;
        mv.registers_table[5] = mv.memory[ip]; //se incicaliza IP
        while(ip < mv.segment_descriptor_table[0].size && error == 0) {
            printf(" \n %X este es el contenido de pos act\n", mv.registers_table[5]);
            //carga de operandos
            pos_act = (char)mv.registers_table[5];
            opB = (char)(((pos_act & 0b11000000) >> 6) & 0b00000011);   //CONSULTAR SI ES NECESARIO. LA ULTIMA MASCARA evita problemas con negativo
            opA = (char)((pos_act & 0b00110000) >> 4);
            cod_op = (char)(pos_act & 0b00011111);

            opB_size =  opB;
            opB_size ^=  0x03;
            opA_size =  opA;
            opA_size ^=  0x03;


            printf(" \n %X este es opAsize\n", opA_size);
            printf(" \n %X este es opBsize\n", opB_size);

            //CARGAMOS EN OPX_CONTENT EL CONTENIDO DE LOS OPERANDOS.
            opB_content = 0x0000;
            opA_content = 0x0000;
            j = 0;
            while (j < opB_size) {
                ip+= 1;
                mv.registers_table[5] = mv.memory[ip]; //nuevo
                pos_act = (char)mv.registers_table[5]; //nuevo
                //opB_content = (opB_content | mv.memory[ip]) << 8;
                opB_content = (opB_content | pos_act) << 8;
                j++;
            }
            opB_content >>= 8;
            j = 0;
            while (j < opA_size) {
                ip+= 1;
                mv.registers_table[5] = mv.memory[ip]; //nuevo
                pos_act = (char)mv.registers_table[5]; //nuevo
                //opA_content = (opA_content | mv.memory[ip]) << 8;
                opA_content = (opA_content | pos_act) << 8;
                j++;
            }
            opA_content >>= 8;


            printf(" \n %X este es opAcontent\n", opA_content);
            printf(" \n %X este es opBcontent\n", opB_content);
            printf("\n %x este es cod op \n", cod_op);
            printf("\n ANTES DE EJECUCION \n");


            /**ACA EJECUTA OPERACION\n
            *
            * que necesitamos?\n
            *\n
            * * codigo de operacion  \n
            * * operando A \n
            * * operando B \n
            * * contenido de A \n
            * * contenido de B \n
            * * tipos de operando \n
            *\n
            * juntando estas cosas, deberiamos poder ejecutar la instruccion\n
            *\n
            * analizar el contenido de cada tipo de operando(registro, inmediato, memoria)\n
            */
            llamado_funcion(&mv, opA, opA_content, opB, opB_content, cod_op, &error);
           // printf("paso mov");
            //Se actualiza IP

            ip += 1;
            mv.registers_table[5] = mv.memory[ip] ; //nuevo
            pos_act = (char)mv.registers_table[5]; //nuevo
           // pos_act = mv.memory[ip];
            //int aux = mv.registers_table[5] & 0xffff0000;
            //mv.registers_table[5] = aux | pos_act;

            printf("\n DESUPUES  DE EJECUCION \n");
            for (int j = 0 ; j <80 ; j++) {
                if(j%10 == 0)
                    printf("\n");
                printf("\t %d",mv.memory[j]);
            }
            printf("\n TABLA DE REGISTROS PA \n");
            for (int j = 0 ; j < 16; j++) {
                if(j%4 == 0)
                    printf("\n");
                printf("\t %X",mv.registers_table[j]);
            }



            }
        }
    printf("\n DESUPUES  DE EJECUCION \n");
    for (int j = 0 ; j <80 ; j++) {
        if(j%10 == 0)
            printf("\n");
        printf("\t %d",mv.memory[j]);
    }
    printf("\n TABLA DE REGISTROS PA \n");
    for (int j = 0 ; j < 16; j++) {
        if(j%4 == 0)
            printf("\n");
        printf("\t %X",mv.registers_table[j]);
    }

    if(dissasembler)
        dissasembler_func(mv);


}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
void set_registers_table(struct VM *mv) {
    (*mv).registers_table[0] = (*mv).segment_descriptor_table[0].base << 16; //corresponde a CS
    (*mv).registers_table[1] = 0x00010000 | (*mv).segment_descriptor_table[1].base; //corresponde a DS
    (*mv).registers_table[2] = 0;
    (*mv).registers_table[3] = 0;
    (*mv).registers_table[4] = 0;
    (*mv).registers_table[5] = (*mv).segment_descriptor_table[0].base << 16; //corresponde a IP
    (*mv).registers_table[6] = 0;
    (*mv).registers_table[7] = 0;
    (*mv).registers_table[8] = 0;
    (*mv).registers_table[9] = 0;
    (*mv).registers_table[10] = 0;
    (*mv).registers_table[11] = 0;
    (*mv).registers_table[12] = 0;
    (*mv).registers_table[13] = 0;
    (*mv).registers_table[14] = 0;
    (*mv).registers_table[15] = 0;
}


void dissasembler_func(struct  VM mv){
    int i=0;
    while(i<mv.segment_descriptor_table[0].size) {
        printf("%X ", mv.memory[i]);
        i++;
    }
    char regs_tags[0x40][4];
    char nemonicos_tags[0X20][5];
    carga_regs(regs_tags);
    carga_nemonics(nemonicos_tags);
    char pos_act = mv.segment_descriptor_table[0].base;
    char opA, opB, cod_op;
    int opA_size, opB_size, j, opA_content, opB_content, registro, offset;
    while(pos_act < mv.segment_descriptor_table[0].size) {
        /// [pos instruccion]  instrucciones en hexa | MNEM  OPA, OPB
        printf("\n[%X] ",(unsigned char)   pos_act);

        opB = (char) (((mv.memory[pos_act] & 0b11000000) >> 6) & 0b00000011);   //CONSULTAR SI ES NECESARIO. LA ULTIMA MASCARA evita problemas con negativo
        opA = (char) ((mv.memory[pos_act] & 0b00110000) >> 4);
        cod_op = (char) (mv.memory[pos_act] & 0b00011111);

        printf("%X ",(unsigned char)   mv.memory[pos_act]);

        opB_size = opB;
        opB_size ^= 0x03;
        opA_size = opA;
        opA_size ^= 0x03;
        opA_content = 0;
        opB_content = 0;
        j = 0;
        while (j < opB_size) {
            pos_act += 1;
            opB_content = (opB_content | mv.memory[pos_act]) << 8;
            printf("%X ", (unsigned char)  mv.memory[pos_act]);
            j++;
        }
        opB_content >>= 8;

        j = 0;
        while (j < opA_size) {
            pos_act += 1;
            opA_content = (opA_content | mv.memory[pos_act]) << 8;
            printf("%X ",(unsigned char)  mv.memory[pos_act]);
            j++;
        }
        opA_content >>= 8;
        printf("\t | %s \t ", nemonicos_tags[cod_op]);

        switch (opA) {
            case 0: {    //memoria
                registro = opA_content >> 16;
                offset = opA_content & 0x00FFFF;
                if (offset == 0)
                    printf("[%s]", regs_tags[registro]);
                else
                    printf("[%s+%d]", regs_tags[registro], offset);
                break;
            }
            case 1: {    //inmediato
                printf("%d", (short int)opA_content);
                break;
            }
            case 2: {    //registro
                printf("%s", regs_tags[(char)opA_content]);
                break;
            }
        }
        switch (opB) {
            case 0: {    //memoria
                registro = opB_content >> 16;
                offset = opB_content & 0x00FFFF;
                if (offset == 0)
                    printf(", [%s]", regs_tags[registro]);
                else
                    printf(", [%s+%d]", regs_tags[registro], offset);
                break;
            }
            case 1: {    //inmediato
                printf(", %d", (short int)opB_content);
                break;
            }
            case 2: {    //registro
                printf(", %s", regs_tags[(char)opB_content]);

                break;
            }
        }
        printf("\n");
        pos_act++;
    }
}

void carga_regs(char regs_tags[0x40][4]){
    strcpy( regs_tags[0], "CS");
    strcpy( regs_tags[1], "DC");
    strcpy( regs_tags[4], "IP");
    strcpy( regs_tags[7], "CC");
    strcpy( regs_tags[8], "AC");
    strcpy( regs_tags[0xA], "EAX");
    strcpy( regs_tags[0xB], "EBX");
    strcpy( regs_tags[0xC], "ECX");
    strcpy( regs_tags[0xD], "EDX");
    strcpy( regs_tags[0xE], "EEX");
    strcpy( regs_tags[0xF], "EFX");
    strcpy( regs_tags[0x1A], "AL");
    strcpy( regs_tags[0x1B], "BL");
    strcpy( regs_tags[0x1C], "CL");
    strcpy( regs_tags[0x1D], "DL");
    strcpy( regs_tags[0x1E], "EL");
    strcpy( regs_tags[0x1F], "FL");
    strcpy( regs_tags[0x2A], "AH");
    strcpy( regs_tags[0x2B], "BH");
    strcpy( regs_tags[0x2C], "CH");
    strcpy( regs_tags[0x2D], "DH");
    strcpy( regs_tags[0x2E], "EH");
    strcpy( regs_tags[0x2F], "FH");
    strcpy( regs_tags[0x3A], "AX");
    strcpy( regs_tags[0x3B], "BX");
    strcpy( regs_tags[0x3C], "CX");
    strcpy( regs_tags[0x3D], "DX");
    strcpy( regs_tags[0x3E], "EX");
    strcpy( regs_tags[0x3F], "FX");
}

void carga_nemonics(char nemonicos_tags[0x20][5]){
    strcpy(nemonicos_tags[0x00],"MOV");
    strcpy(nemonicos_tags[0x01],"ADD");
    strcpy(nemonicos_tags[0x02],"SUB");
    strcpy(nemonicos_tags[0x03],"SWAP");
    strcpy(nemonicos_tags[0x04],"MUL");
    strcpy(nemonicos_tags[0x05],"DIV");
    strcpy(nemonicos_tags[0x06],"CMP");
    strcpy(nemonicos_tags[0x07],"SHL");
    strcpy(nemonicos_tags[0x08],"SHR");
    strcpy(nemonicos_tags[0x09],"AND");
    strcpy(nemonicos_tags[0x0A],"OR");
    strcpy(nemonicos_tags[0x0B],"XOR");
    strcpy(nemonicos_tags[0x0C],"RND");
    strcpy(nemonicos_tags[0x10],"SYS");
    strcpy(nemonicos_tags[0x11],"JMP");
    strcpy(nemonicos_tags[0x12],"JZ");
    strcpy(nemonicos_tags[0x13],"JP");
    strcpy(nemonicos_tags[0x14],"JN");
    strcpy(nemonicos_tags[0x15],"JNZ");
    strcpy(nemonicos_tags[0x16],"JNP");
    strcpy(nemonicos_tags[0x17],"JNN");
    strcpy(nemonicos_tags[0x18],"LDL");
    strcpy(nemonicos_tags[0x19],"LDH");
    strcpy(nemonicos_tags[0x1A],"NOT");
    strcpy(nemonicos_tags[0x1F],"STOP");
}