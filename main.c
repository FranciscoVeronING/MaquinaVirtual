#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Functions.h"
#include "maquina_virtual.h"
#include "Functions.c"
#include "maquina_virtual.c"



/**
typedef struct segments_descriptors{
    short int base, size;
};

char memory[MEMORY_SIZE];
*/
int get_seccion_reg( struct VM mv, int op_content);
void extract_op(int op_content, char *cod_reg, short int *offset);
void load_memory(FILE * file_mv, struct VM mv);

int get_registro(int op, struct VM mv);
void set_registro(int op, int valor, struct VM mv);

int main(int argc, char *argv[]) {

     struct VM mv;
 /*   if (argc != 2) {
        printf("Uso: %s <nombre_del_archivo_binario>\n", argv[0]);
        return 1;
    }
*/
/// arg[0] = .exe; arg([1] .vmx
    // Abrir el archivo en modo de lectura binaria
    FILE *file_mv;
    file_mv = fopen(argv[1], "rb");
    if (file_mv == NULL) {
        perror("Error al abrir el archivo ");
        return 1;
    }
    char header[8];

    // Leer los primeros 6 bytes del archivo
    fread(&header,sizeof(char),8,file_mv);
    //strcat(header,"\0");
    ///VMX241
    printf("%s",header);
    //Valida que la cabecera del archivo esta bien
    if(strncmp(header,"VMX24",5) != 0){// O PONER header != 0xVMX241
        perror("Error al abrir el archivo pito");
        fclose(file_mv);
        return 1;
    }
    else{
        //COMIENZO DE CARGA DE ARCHIVO EN CS (Code Segment)
        //Creacion de tabla de descriptores de segmentos
        //carga de CS e inicializacion de SDT
        load_memory(file_mv,mv);

        //Se inicializa tabla de registros

        //para mi aca va funcion de inicializa tabla
        mv.registers_table[0] = mv.segment_descriptor_table[0].base << 16; //corresponde a CS
        mv.registers_table[1] = mv.segment_descriptor_table[1].base << 16; //corresponde a DS
        mv.registers_table[5] = mv.segment_descriptor_table[0].base << 16; //corresponde a IP

        //EJECUCION
        int i = 0, j = 0, opB_content, opA_content;
        char pos_act = mv.memory[i];
        char opA, opB, cod_op, mask;
        short int opA_size, opB_size;
        while(i <= mv.segment_descriptor_table[0].size && (pos_act != 0xFF) ) {
            //carga de operandos
            opB = (char)(((pos_act & 0b11000000) >> 6) & 0b00000011);   //CONSULTAR SI ES NECESARIO. LA ULTIMA MASCARA evita problemas con negativo
            opA = (char)((pos_act & 0b00110000) >> 4);
            cod_op = (char)(pos_act & 0b00011111);
            opB_size = (short)(~opB);
            opA_size =  (short)(~opA);

            //CARGAMOS EN OPX_CONTENT EL CONTENIDO DE LOS OPERANDOS.
            opB_content = 0x0000;
            opA_content = 0x0000;
            j = 0;
            while (j < opB_size) {
                opB_content = (opB_content | mv.memory[++i]) << 8;
                j++;
            }
            j = 0;
            while (j < opA_size) {
                opA_content = (opA_content | mv.memory[++i]) << 8;
                j++;
            }
            printf("\n %x este es cod op \n", cod_op);
            //ACA EJECUTA OPERACION
            llamado_funcion(mv, opA, opA_content, opB, opB_content, cod_op);
            //opA_content = funcion(opA_content, opB_content); validaria que tipo de operando es
            //opB_content = funcion(opA_content, opB_content); validaria que tipo de operando es
            printf("paso mov");


            /**
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

            //Se actualiza IP
            pos_act = mv.memory[++i];
            int aux = mv.registers_table[5] & 0xffff0000;
            mv.registers_table[5] = aux | pos_act;
            }
        }
    for (int j = 0; j < 50; j++) {
        printf("\n %X",mv.memory[j]);
    }

}

void load_memory(FILE * file_mv, struct VM mv){

    short int size_cs ;
    fread( &size_cs, 1, 2, file_mv);
    int i = 0;
    while(!feof(file_mv)){
         //carga la memoria ram con el codigo de segmento
         fread(&(mv.memory[i]), 1, 1, file_mv);
         i++;
    }
    fclose(file_mv);

//carga de Tabla de descriptores de segmento
    mv.segment_descriptor_table[0].base = 0x0000;
    mv.segment_descriptor_table[0].size = size_cs;
    mv.segment_descriptor_table[1].base = size_cs;
    mv.segment_descriptor_table[1].size = MEMORY_SIZE - size_cs;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

