#include "Vyquon.h"

VyObj NativeFunction(VyObj* args, int num_args){
    return Nil();
}

/* Initialize the IO functions and library */
void LoadIO(){
    NewFunction("native-function", "()", &NativeFunction);
}
