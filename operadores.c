
/*

void get_seccion_reg(VM mv, int op_content);
void extract_op(int op_content, char *cod_reg, short int *offset);
//funcion getoperador
void getOp(){

}
//funcion setOpeador
void setOp(){

}
//  analizar casos
//registro a memoria
//memoria a memoria
//inmediato a memoria

//registro a registro
//memoria a registro
//inmediato a registro


//registro a inmediato (ERROR)

//inmediato a inmediato (ERROR)

//memoria a inmediato (ERROR)

/**
 * pre: dato de a y dato de b ya validados
 * post: asignacion
 * @param registers_table
 * @param opB_content
 * @param opA_content
 *

void set_memoria(VM mv, char opA,char opB,int opA_content,int opB_content){
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
            int index = mv.segment_descriptor_table[cod_regA].base + offsetA;
            mv.memory[index] = (char) mv.memory[value] ;
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
            mv.memory[index] = (opB_content & 0xFF000000);
            index++;
            mv.memory[index] = (opB_content & 0x00FF0000);
            index++;
            mv.memory[index] = (opB_content & 0x0000FF00);
            index++;
            mv.memory[index] = (opB_content & 0x000000FF);
            break;
        }
        case 0x2: {
            ///registro

            int value;
            value = get_seccion_reg(mv,opB_content);

            /// momento de asignar los 4 bytes 1 a 1
            int index = mv.segment_descriptor_table[cod_regA].base + offsetA;
            mv.memory[index] = (value & 0xFF000000);
            index++;
            mv.memory[index] = (value & 0x00FF0000);
            index++;
            mv.memory[index] = (value & 0x0000FF00);
            index++;
            mv.memory[index] = (value & 0x000000FF);
            break;
        }
    }
}

void extract_op(int op_content, char *cod_reg, short int *offset){
    int mask = 0x000F0000;
    *cod_reg = (char)((op_content & mask)>>16);
    *offset = (short int)(op_content & 0x0000FFFF);
}

int get_seccion_reg(VM mv, int op_content){
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


void set_registro(char opA,char opB,int opA_content,int opB_content, VM mv){
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

            mv.register_table[index] = aux;

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
*/