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

void set_SDT(struct VM *mv, unsigned int size_cs);
void set_registers_table(struct VM *mv);
void set_op(int *op_content, char op_size, struct VM* mv);
void carga_regs(char regs_tags[0x40][4]);
void carga_nemonics(char nemonicos_tags[0x20][5]);
void dissasembler_func(struct  VM mv);
void help_tables(struct VM mv);

#endif //VIRTUAL_MACHINE_MAQUINA_VIRTUAL_H
