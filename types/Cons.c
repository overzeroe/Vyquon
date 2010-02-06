#include "Vyquon.h"

/* Empty list */
VyObj Nil(){
    VyCons* cons = VyMalloc(sizeof(VyCons));
    cons->car = None();
    cons->cdr = None();
    return WrapObj(cons, OBJ_CONS);
}

/* Conjoin two objects */
VyObj Cons(VyObj car, VyObj cdr){
    VyCons* cons = (VyCons*) Obj(Nil());
    cons->car = car;
    cons->cdr = cdr;
    return WrapObj(cons, OBJ_CONS);
}

VyObj Cdr(VyCons* cons){
    return cons->cdr;
}
VyObj Car(VyCons* cons){
    return cons->car;
}

bool IsNil(VyObj obj){
    if(Type(obj) == OBJ_CONS){
        VyCons* cons = (VyCons*) Obj(obj);
        return IsNone(cons->car) && IsNone(cons->cdr);
    }
    if(Type(obj) == OBJ_SYM){
        return SymbolEq((VySymbol*) Obj(obj), "nil");
    }
    return false;
}

VyObj ListGet(VyCons* list, int index){
    if(index == 0) return list->car;
    else return ListGet((VyCons*)Obj(list->cdr), index - 1);
}

int ListLen(VyCons* list){
    if (IsNone(list->car) && IsNone(list->cdr)) return 0;
    else return 1 + ListLen((VyCons*) Obj(list->cdr));
}
