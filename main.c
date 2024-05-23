#include <stdio.h>
#include <string.h>
#include "maquina_virtual.h"
#include "Functions.c"
#include "maquina_virtual.c"

int main(int argc, char *argv[]) {
/// arg[0] = .exe; arg[1] .vmx arg[2] "-d"
    struct VM mv;
    int error = 0;
    char *filename_vmx = NULL;
    char *filename_vmi = NULL;
    int option_m = 0;
    int dissassembler_flag = 0;
    FILE *file_mv_vmx;
    FILE *file_mv_vmi;

    unsigned short int size_cs = 0;
    unsigned short int size_ds = 0;
    unsigned short int size_es = 0;
    unsigned short int size_ss = 0;
    unsigned short int size_ks = 0;
    unsigned short int offset_entry_point;
    unsigned int flag_break_point = 0; // -1 si no existe archivo vmi, 0 si existe, 1 cuando se ejecuta la instruccion SYS y se debugea paso a paso

    unsigned int size_memory_p = 16384;

    ///variables para .vmi
    char *registers, *segments_descriptors;
    unsigned char memory_vmi;
    unsigned short int size_memory_vmi;
    int aux;
    for (int i = 0; i < argc; i++) {
        if (strstr(argv[i], ".vmx") != NULL) {
            aux = strlen(argv[i]);
            filename_vmx = (char *) malloc((aux+1)*sizeof (char));
            strcpy(filename_vmx,argv[i]);
        }else
        if (strstr(argv[i], ".vmi") != NULL) {
            aux = strlen(argv[i]);
            filename_vmi = (char *) malloc((aux+1)*sizeof (char));
            strcpy(filename_vmi,argv[i]);
        }else
        if (strstr(argv[i], "m=") != NULL) {
            option_m = atoi(argv[i]+2);
        }else
        if (strstr(argv[i], "-d") != NULL) {
            dissassembler_flag = 1;
        }
    }
    /// Lectura de tamaño de Memoria, solo parametro M para vmx? o para vmi tambien es valido?
    if (option_m) {
        size_memory_p = option_m * 1024;
    }
    mv.size_memory = size_memory_p;
    ///Lectura de .vmx

    if (filename_vmx) {       //si hay un arhivo .vmx
        file_mv_vmx = fopen(filename_vmx, "rb");
        if (file_mv_vmx == NULL) {
            perror("Error al abrir el archivo .vmx \n");
            return 1;
        } else {
            unsigned char header[8];
            // Leer los primeros 8 bytes del archivo
            fread(&header, sizeof(unsigned char), 8, file_mv_vmx);
            //Valida que la cabecera del archivo esta bien
            if (strncmp(header, "VMX24", 5) != 0) {// O PONER header != 0xVMX241
                perror("Error al abrir el archivo , header erroneo");
                fclose(file_mv_vmx);
                return 1;
            } else {
                int version = header[5];
                size_cs = (int)((header[6] << 8 ) | header[7]);
                if (version == 2) {
                    char segment[10];
                    fread(&segment, sizeof(unsigned char), 10, file_mv_vmx);
                    size_ds = (int)((segment[0] << 8 ) | segment[1]);
                    size_es = (int)((segment[2] << 8 ) | segment[3]);
                    size_ss = (int)((segment[4] << 8 ) | segment[5]);
                    size_ks = (int)((segment[6] << 8 ) | segment[7]);
                    offset_entry_point = (int)((segment[8] << 8 ) | segment[9]);
                }
                else {
                    size_ds = size_memory_p - size_cs;
                    offset_entry_point = 0;
                }
            }
            ///Carga de tablas
            set_SDT(&mv, size_cs, size_ds, size_es, size_ss, size_ks, size_memory_p, &error);
            set_registers_table(&mv, size_cs, size_ds, size_es, size_ss, size_ks, offset_entry_point);
            ///CARGA DE MEMORIA
            mv.memory = (unsigned char *) malloc(size_memory_p * sizeof(unsigned char));
            if(mv.memory == NULL)
                printf("Error al crear malloc");
            unsigned char content_cs[size_cs];
           // fread(&content_cs, sizeof (unsigned char), size_cs, file_mv_vmx);
            int j= 0;
            unsigned char aux;
            do{
                fread(&aux, sizeof(unsigned char), 1, file_mv_vmx);
                content_cs[j] = aux;
                j++;
            } while (j< size_cs);

            fread(mv.memory, sizeof (unsigned char), size_ks, file_mv_vmx);
             j = size_ks;
             int i = 0;
            /*while (j < (size_cs+size_ks)){
                mv.memory[j] = content_cs[i];
                j++;
                i++;
            }*/
            do{
                mv.memory[j] = content_cs[i];
                j++;
                i++;
            }while(j < (size_cs+size_ks));

            fclose(file_mv_vmx);
        }
    }
    ///Lectura de .vmi

    if (filename_vmi != NULL) {
        flag_break_point = 0;
        file_mv_vmi = fopen(filename_vmi, "rb");
        if (file_mv_vmi == NULL) {
            perror("Error al abrir el archivo .vmi \n");
            return 1;
        }
        unsigned char header[6];
        fread(&header, sizeof(unsigned char), 6, file_mv_vmi);
        if (strncmp(header, "VMI24", 5) != 0 && header[5] == 1) {
            perror("Error al abrir el archivo , header erroneo");
            fclose(file_mv_vmi);
            return 1;
        } else {
            registers = (char *) malloc(64*sizeof (char));
            segments_descriptors = (char *) malloc(32*sizeof (char));
            fread(&size_memory_vmi, sizeof(unsigned short int), 1, file_mv_vmi); ///preguntar para que corno se usa
            fread(registers, sizeof(char), 64, file_mv_vmi);
            fread(segments_descriptors, sizeof(char), 32, file_mv_vmi);
            if (filename_vmx == NULL) {
                char aux[5];
                aux[4] = '\0';
                int count = 0;
                //aux = (char*) malloc(4*sizeof (char));
                for (int i = 0; i < 16; ++i) {
                    //aux = NULL;
                    //*aux = registers[4 * count];
                    aux[0] = registers[0 + 4 * i];
                    aux[1] = registers[1 + 4 * i];
                    aux[2] = registers[2 + 4 * i];
                    aux[3] = registers[3 + 4 * i];
                  //  strncpy(aux, (registers + (4 * count)), 4);
                    printf("aux: %x\n", aux);
                    mv.registers_table[i] = (int)strtol(aux, NULL, 16);
                   // mv.registers_table[i] = atoi(aux);
                }
                ///copia de Segments_Descriptor_Table
              //  aux = NULL;
                count = 0;
                for (int i = 0; i < 8; ++i) {
                  //  aux = NULL;
                    strncpy(aux, (segments_descriptors + (2 * count)), 2);
                    mv.segment_descriptor_table[i].base = (unsigned short int) atoi(aux);
                   // aux = NULL;
                    count++;
                    strncpy(aux, (segments_descriptors + (2 * count)), 2);
                    mv.segment_descriptor_table[i].size = (unsigned short int) atoi(aux);
                    count++;
                }

                ///CARGA DE MEMORIA

                mv.memory = (unsigned char *) malloc(size_memory_vmi * sizeof(unsigned char));
                int j = 0;
                while (j<size_memory_vmi){
                    fread(&memory_vmi, sizeof(unsigned char), 1, file_mv_vmi);
                    mv.memory[j] = memory_vmi;
                    j++;
                }
            }
            fclose(file_mv_vmi);
        }
    }
    ///EJECUCION
    int  opB_content, opA_content;
     char pos_act;
    char opA, opB, cod_op;
    char opA_size, opB_size;

   int indexIP = (int)((mv.registers_table[5]>>16) & 0xf);
   int index ;
    while (error == 0 && (mv.registers_table[5] &  0x0000FFFF) < (mv.segment_descriptor_table[indexIP].base +mv.segment_descriptor_table[indexIP].size)) {
            //carga de operandos
         index = mv.registers_table[5] & 0x0000ffff;
         pos_act = (char) mv.memory[index];//este ta bom, creo que el problema es que index no lo actualizamos, y siempre va a ser este
         opB = (char) (((pos_act & 0b11000000) >> 6) & 0b00000011);
         opA = (char) ((pos_act & 0b00110000) >> 4);
         cod_op = (char) (pos_act & 0b00011111);

         opB_size = opB;
         opB_size ^= 0x03;
         opA_size = opA;
         opA_size ^= 0x03;

            //CARGAMOS EN OPX_CONTENT EL CONTENIDO DE LOS OPERANDOS.
         opB_content = 0x00000000;
         opA_content = 0x00000000;
         set_op(&opB_content, opB_size, &mv, &error);
         set_op(&opA_content, opA_size, &mv, &error);
            //Se actualiza IP
         mv.registers_table[5] += 1; //SE SACA IP Y LA DE ARRIBA, si es 5, actualiza ip de una
         if (flag_break_point){
             llamado_funcion( &mv, opA, opA_content, opB, opB_content, cod_op, &error, &flag_break_point, filename_vmi);
             SYS(&mv, 0xf, &error, &flag_break_point, filename_vmi);
         }
         else {
             llamado_funcion(&mv, opA, opA_content, opB, opB_content, cod_op, &error, &flag_break_point,filename_vmi);
         }
             //para ver lamemoria y las tablas, debug caserito
            //printf("error = %d", error);
    }

    Errores(error);
    if(dissassembler_flag == 1)
        dissasembler_func(mv);

    free(filename_vmi);
    free(filename_vmx);
    free(mv.memory);
}



