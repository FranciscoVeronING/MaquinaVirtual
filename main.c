#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CANT_SDT 2       //Cantidad de celdas de la tabla de descriptores de segmento
#define MEMORY_SIZE 16384



/**
typedef struct segments_descriptors{
    short int base, size;
};

char memory[MEMORY_SIZE];
*/
typedef struct segments_descriptors{
    short int base, size;
};

typedef struct VM{
    struct segments_descriptors segment_descriptor_table[CANT_SDT];
    char memory[MEMORY_SIZE];
    int registers_table[16];
};
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

        while(i != mv.segment_descriptor_table[0].size && pos_act != 0xff ) {
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


            //ACA EJECUTA OPERACION

            //opA_content = funcion(opA_content, opB_content); validaria que tipo de operando es
            //opB_content = funcion(opA_content, opB_content); validaria que tipo de operando es

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
}

void load_memory(FILE * file_mv, struct VM mv){

short int size_cs = fread((void *) size_cs, 1, 2, file_mv);
int i = 0;
while(!feof(file_mv)){
//carga la memoria ram con el codigo de segmento
fread((void *) mv.memory[i], 1, 1, file_mv);
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

void set_memoria(struct VM mv, char opA,char opB,int opA_content,int opB_content){
    char cod_regA;
    short int offsetA;

    ///cambio de valores en memoria\n
    ///cosas a considerar\n
    ///hay que asignar el contenido de B en A\n
    ///hay que analizar tipos de operando de B\n

    extract_op(opA_content, &cod_regA, &offsetA);

    switch (opB) {
        //ubicar A en memporia ram, agarrar el contenido (invocar funcion que se encargue)--> asignarlo en espacio de memoria de B

        case 0x0: {
            ///memoria
            char cod_regB;
            short int offsetB;
            extract_op(opB_content, &cod_regB, &offsetB);
            ///en posicion de memoria de A, asignar EL CONTENIDO de B\n
            ///( hay que buscarlo en memoria, por eso extraemos los datos\n
            /// hay varios casos que analizar, pero en si el mas completoabarca todos los casos\n
            /// tenemos el codigo de registro de B, por lo tanto, utilizamos get_seccion_reg para saber bien que registro es\n
            /// a ese valor, que ya de por si tiene un offset, hay que agregarle el otro offset\n
            int value = mv.registers_table[cod_regB];
            value = value | offsetB;
            int index = mv.segment_descriptor_table[cod_regA].base + offsetA; //controlar fin de segmento!!!!
            mv.memory[index] = (char) mv.memory[value];
            index++;
            value++;
            mv.memory[index] = (char) mv.memory[value];
            value++;
            index++;
            mv.memory[index] = (char) mv.memory[value];
            value++;
            index++;
            mv.memory[index] = (char) mv.memory[value];

            break;
        }
        case 0x1: {
            ///inmediato
            int index = mv.segment_descriptor_table[cod_regA].base + offsetA;
            mv.memory[index] = 0;
            index++;
            mv.memory[index] = 0;
            index++;
            mv.memory[index] = (char) ((opB_content & 0x0000FF00)>>8);
            index++;
            mv.memory[index] = (char) (opB_content & 0x000000FF);
            break;
        }
        case 0x2: {
            ///registro

            int value;
            value = get_seccion_reg(mv,opB_content);

            /// momento de asignar los 4 bytes 1 a 1
            int index = mv.segment_descriptor_table[cod_regA].base + offsetA;
            mv.memory[index] = (char)((value & 0xFF000000)>>24);
            index++;
            mv.memory[index] = (char)((value & 0x00FF0000)>>16);
            index++;
            mv.memory[index] = (char)((value & 0x0000FF00)>>8);
            index++;
            mv.memory[index] = (char)(value & 0x000000FF);
            break;
        }
    }
}



int get_seccion_reg(struct VM mv, int op_content){
    ///
    ///     para saber el contenido del registro hacemos lo siguiente:\n
    ///     op_content de registro contiene\n
    ///\n
    ///        |00 xx yyyy|\n
    ///\n
    /// 00  es relleno | xx es seccion del registro | yyyy que registro utilizo\n
    ///
    int mask = 0x30;
    char secc;
    secc = (char) ((op_content & mask) >> 4);
    ///
    /// ya tenemos la seccion, ahora hay que buscar el contenido del registro; para ello: \n
    /// ya sabemos que registro de los 16 vamos a usar. debemos entrar a tabla de registros.\n
    /// de allí  agarrar los  4 bytes y analizarlos?
    ///
    int  register_value;
    char index;
    index = (char) (op_content & 0b00001111);
    register_value = mv.registers_table[index];
    switch (secc) {
        case 0x1:{ //caso 4to byte del registro 00 00 00 xx
            register_value = register_value & 0x000000FF;
            break;
        }
        case 0x2:{ // caso 3er  byte del registro 00 00 xx 00
            register_value = register_value & 0x0000FF00;
            break;
        }
        case 0x3:{ // caso 3er y 4to byte del registro  00 00 xx xx
            register_value = register_value & 0x0000FFFF;
            break;
        }
    }
    return register_value;
}

void set_registro(char opA,char opB,int opA_content,int opB_content,struct VM mv){
    //cambio de valores en registro
    char cod_regA;
    int value_register;

    char index;
    index = (char) (opA_content & 0b00001111); // obtenemos que registro se va a utilizar
    value_register = get_seccion_reg(mv,opA_content); //obtenemos la parte del registro a modificar

    switch(opB){
        case 0x0:{ //memoria
            int aux=0;
            char cod_regB;
            short int offsetB;
            extract_op(opB_content, &cod_regB, &offsetB);
            ///el regstro de A, asignar EL CONTENIDO de B\n
            ///( hay que buscarlo en memoria, por eso extraemos los datos\n
            /// hay varios casos que analizar, pero en si el mas completoabarca todos los casos\n
            /// tenemos el codigo de registro de A, por lo tanto, utilizamos get_seccion_reg para saber bien que registro es\n
            /// a ese valor, que ya de por si tiene un offset, hay que agregarle el otro offset\n
            int value = mv.registers_table[cod_regB];
            value = value | offsetB;
            aux | mv.memory[value] << 8;
            value ++;
            aux | mv.memory[value] << 8;
            value ++;
            aux | mv.memory[value] << 8;
            value ++;
            aux | mv.memory[value];

            mv.registers_table[index] = aux;

            break;
        }
        case 0x1: { //inmediato
            /// cosas a conteplar
            /// el inmediato pisa lo almacenado en el registro, por lo tanto es solo una asignacion
            // mv.register_table[index]  = value_register & opA_content;
            int mask = 0x30;
            char secc;
            secc = (char) ((opA_content & mask) >> 4);
            switch (secc) {
                case 0x0:{ //caso enter0, el registro xx xx xx xx
                    mv.registers_table[index] =  (int) opB_content;
                    break;
                }
                case 0x1:{ //caso 4to byte del registro 00 00 00 xx
                    mv.registers_table[index] = (char) opB_content;
                    break;
                }
                case 0x2:{ // caso 3er  byte del registro 00 00 xx 00
                    mv.registers_table[index] = ((char) opB_content << 8);
                    break;
                }
                case 0x3:{ // caso 3er y 4to byte del registro  00 00 xx xx
                    mv.registers_table[index] = (short int) opB_content;
                    break;
                }
            }
            break;
        }
        case  0x2:{    //registro
            int mask = 0x30;
            int value_registerB;
            char indexB;
            indexB = (char) (opB_content & 0b00001111); // obtenemos que registro se va a utilizar
            value_registerB = get_seccion_reg(mv,opB_content); //obtenemos la parte del registro a insertar
            char seccB;
            seccB = (char) ((opB_content & mask) >> 4);
            if (seccB == 0x2) //si nos pasan AH de un registro
                value_registerB >> 8;
            char secc;
            secc = (char) ((opA_content & mask) >> 4);
            switch (secc) {
                case 0x0:{ //caso entero, el registro xx xx xx xx
                    mv.registers_table[index] = (int) value_registerB;
                    break;
                }
                case 0x1:{ //caso 4to byte del registro 00 00 00 xx
                    mv.registers_table[index] = (char) value_registerB;
                    break;
                }
                case 0x2:{ // caso 3er  byte del registro 00 00 xx 00
                    mv.registers_table[index] = ((char) value_registerB << 8);
                    break;
                }
                case 0x3:{ // caso 3er y 4to byte del registro  00 00 xx xx
                    mv.registers_table[index] = (short int) value_registerB;
                    break;
                }
            }
            break;
        }
    }
}


int get_memoria(int pointer, struct VM mv){
   ///hay 2 opciones, si es memoria directa, o si es la memoria de un registro
    int value = 0;
    int index = pointer & 0x0000FFFF; //OFFSET
    if(index >= (mv.segment_descriptor_table[pointer & 0xFFFF0000].size - 4)){
       value = (int)mv.memory[index];
       value |= mv.memory[index + 1] << 8;
       value |= mv.memory[index + 2] << 16;
       value |= mv.memory[index + 3] << 24;
    }
    else{
        perror("te caiste del segmento pa");
    }
    return value;
}

void extract_op(int op_content, char *cod_reg, short int *offset){
    int mask = 0x000F0000;
    *cod_reg = (char)((op_content & mask)>>16);
    *offset = (short int)(op_content & 0x0000FFFF);
}
*/

int value_op(int op_content, char op_type, struct VM mv){  //obtiene el valor del operando
    int value,cod_seg;
    switch(op_type){
        case 0: {   //caso de memoria
            char code_reg = (char) ((op_content & 0x0F0000) >> 16);
            int offset_1 = op_content & 0x00FFFF;
            int offset_2 = mv.registers_table[code_reg] & 0x0000FFFF;
            int pointer = (int)((mv.registers_table[code_reg] & 0xFFFF0000) + (offset_1 + offset_2));
            value = get_memoria(pointer, mv);
            break;
        }
        case 1:  //caso inmediato
            value = op_content;
            break;
        case 2: //caso registro
            value = get_registro(op_content,mv);
            break;
    }
    return value;
}







int get_registro(int op, struct VM mv){
//obtiene el valor de un registro
    int cod_reg, sec_reg, valor;
    sec_reg = op >> 4 & 0x3;        //almacena el tipo de registro
    cod_reg = op & 0xF;             //almacena el registro
    switch (sec_reg) {
        case 0:{
            valor = mv.registers_table[cod_reg];
            break;
        }
        case 1:{
            valor = mv.registers_table[cod_reg] & 0xFF;
            break;
        }
        case 2:{
            valor = mv.registers_table[cod_reg] & 0xFF00;
            break;
        }
        case 3:{
            valor = mv.registers_table[cod_reg] & 0xFFFF;
            break;
        }
    }
    return valor;
}

void set_registro(int op, int valor, struct VM mv){
    //cambia el valor de un registro
    int cod_reg, sec_reg;
    sec_reg = op >> 4 & 0x3;        //almacena el tipo de registro
    cod_reg = op & 0xF;             //almacena el registro
    switch (sec_reg) {
        case 0:{
            mv.registers_table[cod_reg] = valor; //Caso EAX
            break;
        }
        case 1:{
            mv.registers_table[cod_reg] = (mv.registers_table[cod_reg] & 0xFFFFFF00) | valor; //se quieren mantener los 24 bits y modificar los ultimos 8 (caso AL)
            break;
        }
        case 2:{
            mv.registers_table[cod_reg] = (mv.registers_table[cod_reg] & 0xFFFF00FF) | (valor << 8); // Caso modificar AH
            break;
        }
        case 3:{
            mv.registers_table[cod_reg] = (mv.registers_table[cod_reg] & 0xFFFF0000) | valor; //Caso modificar AX
            break;
        }
    }
}


