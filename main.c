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
    int option_m = -1;
    int dissassembler_flag = 0;
    FILE *file_mv_vmx;
    FILE *file_mv_vmi;

    unsigned short int size_cs = 0;
    unsigned short int size_ds = 0;
    unsigned short int size_es = 0;
    unsigned short int size_ss = 0;
    unsigned short int size_ks = 0;
    unsigned short int offset_entry_point;
    unsigned int flag_break_point = -1; // -1 si no existe archivo vmi, 0 si existe, 1 cuando se ejecuta la instruccion SYS y se debugea paso a paso

    unsigned char filename_vmx_content;
    int size_memory_p = 16384;

    ///variables para .vmi
    char *registers, *segments_descriptors;
    unsigned char memory_vmi;
    unsigned short int size_memory_vmi;
    int aux;
    for (int i = 1; i < argc; i++) {
        if (strstr(argv[i], ".vmx") != NULL) {
            aux = strlen(argv[i]);
            filename_vmx = (char *) malloc((aux+1)*sizeof (char));
            strcpy(filename_vmx,argv[i]);
        } else if (strstr(argv[i], ".vmi") != NULL) {
            aux = strlen(argv[i]);
            filename_vmi = (char *) malloc((aux+1)*sizeof (char));
            strcpy(filename_vmi,argv[i]);
        } else if (strncmp(argv[i], "m=", 2) == 0) {
            option_m = atoi(argv[i]);
        } else if (strcmp(argv[i], "-d") == 0) {
            dissassembler_flag = 1;
        }
    }

    /// Lectura de tamaño de Memoria, solo parametro M para vmx? o para vmi tambien es valido?
    if (option_m) {
        size_memory_p = option_m * 1024;
    }

    ///Lectura de .vmx

    if (filename_vmx) {       //si hay un arhivo .vmx
        file_mv_vmx = fopen(filename_vmx, "rb");
        if (file_mv_vmx == NULL) {
            perror("Error al abrir el archivo .vmx \n");
            return 1;
        } else {
            unsigned char header[5];
            // Leer los primeros 6 bytes del archivo
            fread(&header, sizeof(unsigned char), 5, file_mv_vmx);
            //Valida que la cabecera del archivo esta bien
            if (strncmp(header, "VMX24", 5) != 0) {// O PONER header != 0xVMX241
                perror("Error al abrir el archivo , header erroneo");
                fclose(file_mv_vmx);
                return 1;
            } else {
                int version;
                fread(&version, sizeof(unsigned char), 1, file_mv_vmx);
                if (version == 2) {
                    fread(&size_cs, sizeof(unsigned short int), 1, file_mv_vmx);
                    fread(&size_ds, sizeof(unsigned short int), 1, file_mv_vmx);
                    fread(&size_es, sizeof(unsigned short int), 1, file_mv_vmx);
                    fread(&size_ss, sizeof(unsigned short int), 1, file_mv_vmx);
                    fread(&size_ks, sizeof(unsigned short int), 1, file_mv_vmx);
                    fread(&offset_entry_point, sizeof(unsigned short int), 1, file_mv_vmx);
                } else
                    if (version == 1) {
                        fread(&size_cs, sizeof(unsigned short int), 1, file_mv_vmx);
                }
            }

            ///CARGA DE MEMORIA

            mv.memory = (unsigned char *) malloc(size_memory_p * sizeof(unsigned char));
            int j= 0;
            while (!feof(file_mv_vmx)) {
                fread(&filename_vmx_content, sizeof(unsigned char), 1, file_mv_vmx);
                mv.memory[j] = filename_vmx_content;
                j++;
            }
            fclose(file_mv_vmx);

            set_SDT(&mv, size_cs, size_ds, size_es, size_ss, size_ks, size_memory_p, &error);
            set_registers_table(&mv, size_cs, size_ds, size_es, size_ss, size_ks, offset_entry_point);
        }
    }

    ///Lectura de .vmi

    if (filename_vmi) {
        flag_break_point = 0;
        file_mv_vmi = fopen(filename_vmi, "rb");
        if (file_mv_vmi == NULL) {
            perror("Error al abrir el archivo .vmi \n");
            return 1;
        }
        unsigned char header[6];
        fread(&header, sizeof(unsigned char), 5, file_mv_vmi);
        if (strncmp(header, "VMI24", 5) != 0) {
            perror("Error al abrir el archivo , header erroneo");
            fclose(file_mv_vmi);
            return 1;
        } else {
            fread(&size_memory_vmi, sizeof(unsigned short int), 1, file_mv_vmi); ///preguntar para que corno se usa
            fread(registers, sizeof(char), 64, file_mv_vmi);
            fread(segments_descriptors, sizeof(char), 32, file_mv_vmi);
            if (filename_vmx == NULL) {
                char *aux;
                int count = 0;
                for (int i = 0; i < 16; ++i) {
                    aux = NULL;
                    strncpy(aux, (registers + (4 * count)), 4);
                    mv.registers_table[i] = atoi(aux);
                    count++;
                }
                ///copia de Segments_Descriptor_Table
                aux = NULL;
                count = 0;
                for (int i = 0; i < 8; ++i) {
                    aux = NULL;
                    strncpy(aux, (segments_descriptors + (2 * count)), 2);
                    mv.segment_descriptor_table[i].base = (unsigned short int) atoi(aux);
                    aux = NULL;
                    count++;
                    strncpy(aux, (segments_descriptors + (2 * count)), 2);
                    mv.segment_descriptor_table[i].size = (unsigned short int) atoi(aux);
                    count++;
                }

                ///CARGA DE MEMORIA

                mv.memory = (unsigned char *) malloc(size_memory_vmi * sizeof(unsigned char));
                int j = 0;
                while (!feof(file_mv_vmi)) {
                    fread(&memory_vmi, sizeof(unsigned char), 1, file_mv_vmi);
                    mv.memory[j] = memory_vmi;
                    j++;
                }
            }
            fclose(file_mv_vmi);
        }

        ///EJECUCION
        int j, opB_content, opA_content;
        char pos_act;
        char opA, opB, cod_op;
        char opA_size, opB_size;
        int error = 0;

        int indexCS = mv.registers_table[0] >> 16;

        while (error == 0 && mv.registers_table[5] < mv.segment_descriptor_table[indexCS].size) {
            //carga de operandos
            pos_act = (char) mv.memory[mv.registers_table[5]];
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
            set_op(&opB_content, opB_size, &mv);
            set_op(&opA_content, opA_size, &mv);

            //Se actualiza IP
            mv.registers_table[5] += 1; //SE SACA IP Y LA DE ARRIBA}
            if (flag_break_point){
                llamado_funcion( &mv, opA, opA_content, opB, opB_content, cod_op, &error, &flag_break_point, filename_vmi);
                SYS(&mv, 0x46, &error, &flag_break_point, filename_vmi);
            }
            else
                llamado_funcion( &mv, opA, opA_content, opB, opB_content, cod_op, &error, &flag_break_point, filename_vmi);
            ///help_tables(mv); //para ver lamemoria y las tablas, debug caserito
        }
        Errores(error);
    }
    if(dissassembler_flag == 1)
        dissasembler_func(mv);

    free(filename_vmi);
    free(filename_vmx);
    free(mv.memory);
}



