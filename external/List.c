#include "Vyquon.h"

VyObj MakeCons(VyObj*, int);

/* Initialize the IO functions and library */
void LoadList(){
    NewFunction("cons", "(x y)", &MakeCons);
}

VyObj MakeCons(VyObj* values, int num_args){
    return Cons(values[0], values[1]);
}
