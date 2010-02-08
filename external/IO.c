#include "Vyquon.h"

VyObj PrintObjFun(VyObj* args, int num_args){
    PrintObj(stdout, args[0]);
    printf(" ");
    fflush(stdout);
    return args[0];
}

/* Initialize the IO functions and library */
void LoadIO(){
    NewFunction("print", "(a)", &PrintObjFun);
}
