#include "Vyquon.h"

/* Return the objects representing true and false (symbols of them) */
VyObj TrueObj(){
    return CreateSymbol("true");
}
VyObj FalseObj(){
    return CreateSymbol("false");
}

/* Check for truthity of an object */
bool IsTrue(VyObj x){
    return !IsFalse(x);
}
bool IsFalse(VyObj x){
    return (IsNil(x) || ObjEq(x, SymbolFalse));
}

VyObj AndFun(VyObj* objs, int num){
    int i;
    for(i = 0; i < num; i++)
        if(!IsTrue(objs[i]))
            return FalseObj();
    return TrueObj();
}
VyObj OrFun(VyObj* objs, int num){
    int i;
    for(i = 0; i < num; i++)
        if(!IsFalse(objs[i]))
            return TrueObj();
    return FalseObj();
}
VyObj NotFun(VyObj* objs, int num){
    if(IsTrue(objs[0]))
        return FalseObj();
    else
        return TrueObj();
}


void LoadBool(){
    NewFunction("and", "(... vals)", &AndFun);
    NewFunction("or", "(... vals)", &OrFun);
    NewFunction("not", "(val)", &NotFun);
}
