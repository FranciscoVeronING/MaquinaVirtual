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
void set_registro(int op, int valor, struct VM* mv);

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
        char aux;
        fread(&aux, sizeof(char), 1, file_mv);
        while(!feof(file_mv)){
            //carga la memoria ram con el codigo de segmento
            mv.memory[i] = aux;
            i++;
            fread(&aux, sizeof(char), 1, file_mv);
        }
        fclose(file_mv);
        scanf("%c",&aux);

//carga de Tabla de descriptores de segmento
        mv.segment_descriptor_table[0].base = 0x0000;
        mv.segment_descriptor_table[0].size = size_cs;
        mv.segment_descriptor_table[1].base = size_cs;
        mv.segment_descriptor_table[1].size = MEMORY_SIZE - size_cs;


        //Se inicializa tabla de registros

        //para mi aca va funcion de inicializa tabla
        mv.registers_table[0] = mv.segment_descriptor_table[0].base << 16; //corresponde a CS
        mv.registers_table[1] = 0x00010000 | mv.segment_descriptor_table[1].base; //corresponde a DS
        mv.registers_table[5] = mv.segment_descriptor_table[0].base << 16; //corresponde a IP
        printf("%x DS\n", mv.registers_table[1]);
        //EJECUCION
        int ip = mv.segment_descriptor_table[0].base, j = 0, opB_content, opA_content;
        char pos_act = mv.memory[ip];
        char opA, opB, cod_op, mask;
        char opA_size, opB_size;
        while(ip <= mv.segment_descriptor_table[0].size && (pos_act != 0xFF) ) {
            printf(" \n %X este es el contenido de pos act\n", pos_act);
            //carga de operandos
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
                opB_content = (opB_content | mv.memory[ip]) << 8;
                j++;
            }
            opB_content >>= 8;
            j = 0;
            while (j < opA_size) {
                ip+= 1;
                opA_content = (opA_content | mv.memory[ip]) << 8;
                j++;
            }
            opA_content >>= 8;

            /*
            printf(" \n %X este es opAcontent\n", opA_content);
            printf(" \n %X este es opBcontent\n", opB_content);
            printf("\n %x este es cod op \n", cod_op);
            printf("\n ANTES DE EJECUCION \n");
            */

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
            llamado_funcion(&mv, opA, opA_content, opB, opB_content, cod_op);
           // printf("paso mov");
            //Se actualiza IP
            ip += 1;
            pos_act = mv.memory[ip];
            int aux = mv.registers_table[5] & 0xffff0000;
            mv.registers_table[5] = aux | pos_act;
            }
        }
    printf("\n DESUPUES  DE EJECUCION \n");
    for (int j = 0 ; j < 60; j++) {
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////

