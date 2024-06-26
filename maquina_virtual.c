#include "maquina_virtual.h"



void set_SDT(struct VM *mv, unsigned short int size_cs, unsigned short int size_ds, unsigned short int size_es, unsigned short int size_ss, unsigned short int size_ks,unsigned int size_mp, int *error){
    unsigned int aux = size_ks+size_cs+size_es+size_ds+size_ss;
    if( aux > size_mp){
       // printf("memo %d", size_mp);
        *error = 4;
    }
    else{
        int index = 0, base = 0;
        if(size_ks !=0){
            (*mv).segment_descriptor_table[index].base = base;
            (*mv).segment_descriptor_table[index].size = size_ks;
            base += size_ks;
            index++;
        }
        if(size_cs !=0){
            (*mv).segment_descriptor_table[index].base = base;
            (*mv).segment_descriptor_table[index].size = size_cs;
            base += size_cs;
            index++;
        }
        if(size_ds !=0){
            (*mv).segment_descriptor_table[index].base = base;
            (*mv).segment_descriptor_table[index].size = size_ds;
            base += size_ds;
            index++;
        }
        if(size_es !=0){
            (*mv).segment_descriptor_table[index].base = base;
            (*mv).segment_descriptor_table[index].size = size_es;
            base += size_es;
            index++;
        }
        if(size_ss !=0){
            (*mv).segment_descriptor_table[index].base = base;
            (*mv).segment_descriptor_table[index].size = size_ss;
        }
    }
}

void set_registers_table(struct VM *mv, unsigned short int size_cs, unsigned short int size_ds, unsigned short int size_es, unsigned short int size_ss, unsigned short int size_ks, unsigned short int offset_entry_point) {
    int index = 0;
    if (size_ks == 0)
        (*mv).registers_table[4] = -1;
    else {
        (*mv).registers_table[4] = index << 16;
        index++;
    }
    if (size_cs == 0) {
        (*mv).registers_table[0] = -1;
    } else {
        (*mv).registers_table[0] = index << 16;
        (*mv).registers_table[5] = (index << 16) + (*mv).segment_descriptor_table[index].base + offset_entry_point; //El registro IP apunta a CS
        index++;
    }
    if (size_ds == 0)
        (*mv).registers_table[1] = -1;
    else {
        (*mv).registers_table[1] = index << 16;
        index++;
    }
    if (size_es == 0)
        (*mv).registers_table[2] = -1;
    else {
        (*mv).registers_table[2] = index << 16;
        index++;
    }
    if (size_ss == 0) {
        (*mv).registers_table[3] = -1;
        (*mv).registers_table[6] = -1;
    } else {
        (*mv).registers_table[3] = index << 16;
        (*mv).registers_table[6] = (index << 16) + mv->segment_descriptor_table[index].size; //el registro Sp apunta altope de la pila
    }

    for (index = 7; index < 16; ++index) {
        (*mv).registers_table[index] = 0;
    }
}

void set_op(int *op_content, char op_size, struct VM* mv) {
    char pos_act;
    int j = 0, index;
    while (j < op_size) {
        (*mv).registers_table[5]  += 1; //SE SACA IP Y LA DE ARRIBA
        index = (*mv).registers_table[5] & 0x0000ffff;
        pos_act = (char) (*mv).memory[index];
        (*op_content) <<= 8;
        (*op_content) += (unsigned char)pos_act;
        j++;
    }
}

void dissasembler_func(struct  VM mv){

    char regs_tags[0x40][4];
    char nemonicos_tags[0X20][5];
    carga_regs(regs_tags);
    carga_mnemonics(nemonicos_tags);

    char opA, opB, cod_op;
    int opA_size, opB_size, j, opA_content, opB_content, registro, offset;

    ////Muestra de Constants Segment
    if(mv.registers_table[4] != -1) {
        printf("\n /////////////CONSTANTS  SEGMENT/////////////");
        int indexKS = mv.registers_table[4] >> 16;
        unsigned int pos_act_ks = (mv.segment_descriptor_table[indexKS].base);
        unsigned int strsize;
        int i;
        while(mv.segment_descriptor_table[indexKS].size != 0 && pos_act_ks < (mv.segment_descriptor_table[indexKS].size + (mv.segment_descriptor_table[indexKS].base))) {
            printf("\n [%04X] ", (unsigned int) pos_act_ks);
            char auxstr[100];
            i = 0;
            while (mv.memory[pos_act_ks] != '\0') {
                auxstr[i] = (char)mv.memory[pos_act_ks];
                i++;
                pos_act_ks++;
            }
            pos_act_ks++;
            auxstr[i] = '\0'; //aca se guarda el string
            strsize = strlen(auxstr);
            if (strsize > 7) {   //caso para strings largos
                for (int k = 0; k < 6; ++k) {
                    printf("%02X ", (unsigned char) auxstr[k]);
                }
                printf("..");
            } else {
                for (int k = 0; k < strsize; ++k)
                    printf("%02X ", (unsigned char) auxstr[k]);
               }
            printf("| %s ", auxstr);
        }
    }

    printf("\n ////////////////CODE SEGMENT////////////////");
        ////Muestra de Code Segment
    int index = mv.registers_table[0] >>16;
    unsigned int pos_act =  (mv.segment_descriptor_table[index].base);
    while(mv.segment_descriptor_table[index].size != 0 && pos_act < (mv.segment_descriptor_table[index].size + (mv.segment_descriptor_table[index].base))) {
        /// [pos instruccion]  instrucciones en hexa | MNEM  OPA, OPB
        if(pos_act == mv.entry_point)
            printf("\n>[%04X] ",(unsigned int)   pos_act);
        else
            printf("\n [%04X] ",(unsigned int)   pos_act);

        opB = (char) (((mv.memory[pos_act] & 0b11000000) >> 6) & 0b00000011);
        opA = (char) ((mv.memory[pos_act] & 0b00110000) >> 4);
        cod_op = (char) (mv.memory[pos_act] & 0b00011111);

        printf("%02X ",(unsigned char) mv.memory[pos_act]);

        opB_size = (unsigned char) opB;
        opB_size ^= 0x03;
        opA_size = (unsigned char) opA;
        opA_size ^= 0x03;
        opA_content = 0;
        opB_content = 0;
        j = 0;
        while (j < opB_size) {
            pos_act += 1;
            opB_content <<= 8;
            opB_content += mv.memory[pos_act];
            printf("%02X ", (unsigned char)  mv.memory[pos_act]);
            j++;
        }

        j = 0;
        while (j < opA_size) {
            pos_act += 1;
            opA_content <<= 8;
            opA_content += mv.memory[pos_act];
            printf("%02X ",(unsigned char)  mv.memory[pos_act]);
            j++;
        }

        int cant = opB_size + opA_size + 1;
        while(cant <= 7){
            printf("   ");
            cant++;
        }
        printf("| %s ", nemonicos_tags[cod_op]);


        switch (opA) {
            case 0: {    //memoria

                registro = (opA_content >> 16) & 0x00000f;
                offset = opA_content & 0x00FFFF;
                offset <<= 16;
                offset >>= 16;
                if (offset == 0)
                    printf("%c[%s]", setlabel(opA_content),regs_tags[registro]);
                else if (offset > 0)
                    printf("%c[%s+%d]", setlabel(opA_content), regs_tags[registro], offset);
                else
                    printf("%c[%s%d]", setlabel(opA_content), regs_tags[registro], offset);
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
            default:{
                break;
            }
        }
        if(opA != 3)
            printf(",");
        switch (opB) {
            case 0: {    //memoria
                registro = (opB_content >> 16) & 0x00000f;
                offset = opB_content & 0x00FFFF;
                offset <<= 16;
                offset >>= 16;
                if (offset == 0)
                    printf("%c[%s]", setlabel(opB_content),regs_tags[registro]);
                else if(offset > 0)
                    printf("%c[%s+%d]", setlabel(opB_content), regs_tags[registro], offset);
                else
                    printf("%c[%s%d]", setlabel(opB_content), regs_tags[registro], offset);
                break;
            }
            case 1: {    //inmediato
                printf(" %d", (short int)opB_content);
                break;
            }
            case 2: {    //registro
                printf(" %s", regs_tags[(char)opB_content]);

                break;
            }
            default:{
                break;
            }
        }
        pos_act++;
    }
    printf("\n ////////////////////////////////////////////\n");
}

unsigned char setlabel(int content) {
    content >>= 20;
    content = (content & 0x0000000f);

    char aux;
    switch (content) {
        case 0 : {
             aux = 'l';
             break;
        }
        case 2:{
            aux = 'w';
            break;
        }
        default:{
            aux = 'b';
            break;
        }
    }
    return aux;
}

void carga_regs(char regs_tags[0x40][4]){
    strcpy( regs_tags[0], "CS");
    strcpy( regs_tags[1], "DS");
    strcpy( regs_tags[2], "ES");
    strcpy( regs_tags[3], "SS");
    strcpy( regs_tags[4], "KS");
    strcpy( regs_tags[5], "IP");
    strcpy( regs_tags[6], "SP");
    strcpy( regs_tags[7], "BP");
    strcpy( regs_tags[8], "CC");
    strcpy( regs_tags[9], "AC");
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

void carga_mnemonics(char nemonicos_tags[0x20][5]){
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
    strcpy(nemonicos_tags[0x1B],"PUSH");
    strcpy(nemonicos_tags[0x1C],"POP");
    strcpy(nemonicos_tags[0x1D],"CALL");
    strcpy(nemonicos_tags[0x1E],"RET");
    strcpy(nemonicos_tags[0x1F],"STOP");
}

/////////////////////////////////////////////////////////////////////////////////
void help_tables(struct VM mv) {
    char regs_tags[0x40][4];
    carga_regs(regs_tags);
    printf("\n Memoria \n");
    for (int j = 0 ; j <200 ; j++) {
        if(j%10 == 0)
            printf("\n");
        printf(" [%02X] %02X  ",j,mv.memory[j]);
    }
    printf("\n TABLA DE REGISTROS  \n");
    for (int j = 0 ; j < 16; j++) {
        if(j%4 == 0)
            printf("\n");
        printf("\t[%s] %08X",regs_tags[j],mv.registers_table[j]);

    }
}
/////////////////////////////////////////////////////////////////////////////////