#include <stdio.h>
#include <string.h>
#include "maquina_virtual.h"
#include "Functions.c"
#include "maquina_virtual.c"

int main(int argc, char *argv[]) {
/// arg[0] = .exe; arg[1] .vmx arg[2] "-d"
    FILE *file_mv;
    int dissasembler = 0; //flag de disassembler
    struct VM mv;

    if(argv[2] && strcmp(argv[2],"-d") == 0)
        dissasembler = 1;
    //apertura de archivo
    file_mv = fopen(argv[1], "rb");
    if (file_mv == NULL) {
        perror("Error al abrir el archivo ");
        return 1;
    }
   unsigned char header[8];
    // Leer los primeros 6 bytes del archivo
    fread(&header,sizeof(unsigned char),8,file_mv);
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
        unsigned int size_cs;
        size_cs = (int)((header[6] << 8 ) | header[7]);
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
        //carga de Tabla de descriptores de segmento
        set_SDT(&mv, size_cs);
        //Se inicializa tabla de registros
        set_registers_table(&mv);
       ///EJECUCION
        mv.registers_table[5] = mv.segment_descriptor_table[0].base; //INICIALIZACION
        int j, opB_content, opA_content;
        char pos_act;
        char opA, opB, cod_op;
        char opA_size, opB_size;
        int error = 0;

        while(error == 0 && mv.registers_table[5] < mv.segment_descriptor_table[0].size) {
            //carga de operandos
            pos_act = (char) mv.memory[mv.registers_table[5]];
            opB = (char)(((pos_act & 0b11000000) >> 6) & 0b00000011);
            opA = (char)((pos_act & 0b00110000) >> 4);
            cod_op = (char)(pos_act & 0b00011111);

            opB_size =  opB;
            opB_size ^=  0x03;
            opA_size =  opA;
            opA_size ^=  0x03;

            //CARGAMOS EN OPX_CONTENT EL CONTENIDO DE LOS OPERANDOS.
            opB_content = 0x00000000;
            opA_content = 0x00000000;
            set_op(&opB_content,opB_size,&mv);
            set_op(&opA_content,opA_size,&mv);

            //Se actualiza IP
            mv.registers_table[5]  += 1; //SE SACA IP Y LA DE ARRIBA}
            llamado_funcion(&mv, opA, opA_content, opB, opB_content, cod_op, &error);
            ///help_tables(mv); //para ver lamemoria y las tablas, debug caserito
        }
        Errores(error);
    }
    if(dissasembler)
        dissasembler_func(mv);
}