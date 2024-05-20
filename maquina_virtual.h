#ifndef VIRTUAL_MACHINE_MAQUINA_VIRTUAL_H
#define VIRTUAL_MACHINE_MAQUINA_VIRTUAL_H

#define CANT_SDT  8        //Cantidad de celdas de la tabla de descriptores de segmento



typedef struct segments_descriptors{
    unsigned short int base, size;
}SegmentsDescriptors;

typedef struct VM{
    struct segments_descriptors segment_descriptor_table[CANT_SDT];
    unsigned char *memory;
    int registers_table[16];
    unsigned int size_memory;
}VirtualMachine;

void set_SDT(struct VM *mv, unsigned short int size_cs, unsigned short int size_ds, unsigned short int size_es, unsigned short int size_ss, unsigned short int size_ks,unsigned int size_mp, int *error);
void set_registers_table(struct VM *mv, unsigned short int size_cs, unsigned short int size_ds, unsigned short int size_es, unsigned short int size_ss, unsigned short int size_ks, unsigned short int offset_entry_point);
void set_op(int *op_content, char op_size, struct VM* mv);
void carga_regs(char regs_tags[0x40][4]);
void carga_mnemonics(char nemonicos_tags[0x20][5]);
void dissasembler_func(struct  VM mv);
unsigned char setlabel(int content);
void help_tables(struct VM mv);
#endif //VIRTUAL_MACHINE_MAQUINA_VIRTUAL_H
