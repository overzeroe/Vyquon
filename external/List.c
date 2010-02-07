#include "Vyquon.h"

VyObj MakeCons(VyObj*, int);
VyObj MakeList(VyObj*, int);

/* Initialize the IO functions and library */
void LoadList(){
    NewFunction("cons", "(x y)", &MakeCons);
    NewFunction("list", "(... all)", &MakeList);
}

VyObj MakeCons(VyObj* values, int num_args){
    return Cons(values[0], values[1]);
}
VyObj MakeList(VyObj* values, int num_args){
    VyObj lst = Nil();
    int i;
    for(i = num_args - 1; i >= 0; i--)
        lst = Cons(values[i], lst);
    return lst;
}
