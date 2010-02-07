#include "Vyquon.h"

VyObj NativeFunction(VyObj* args, int num_args){
    printf("Inside a native function.\n");
    return Nil();
}

/* Initialize the IO functions and library */
void LoadIO(){
    NewFunction("native-function", "()", &NativeFunction);
}
