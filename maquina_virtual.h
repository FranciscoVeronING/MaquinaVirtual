//
// Created by Fran Vergon on 6/4/2024.
//
#ifndef VIRTUAL_MACHINE_MAQUINA_VIRTUAL_H
#define VIRTUAL_MACHINE_MAQUINA_VIRTUAL_H

#define CANT_SDT 2       //Cantidad de celdas de la tabla de descriptores de segmento
#define MEMORY_SIZE 16384



typedef struct segments_descriptors{
    short int base, size;
}SegmentsDescriptors;

typedef struct VM{
    struct segments_descriptors segment_descriptor_table[CANT_SDT];
    unsigned char memory[MEMORY_SIZE];
    int registers_table[16];
}VirtualMachine;


#endif //VIRTUAL_MACHINE_MAQUINA_VIRTUAL_H
