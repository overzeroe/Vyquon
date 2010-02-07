#include "Vyquon.h"

/* This can return an empty list or the symbol 'nil */
VyObj nil_symbol = {type: {size: 0, name: NULL}, obj: NULL};

VyObj Nil(){
    if(IsNone(nil_symbol))
        nil_symbol = CreateSymbol("nil");
    return nil_symbol;
}

/* Conjoin two objects */
VyObj Cons(VyObj car, VyObj cdr){
    VyCons* cons = VyMalloc(sizeof(VyCons));
    cons->car = car;
    cons->cdr = cdr;
    return WrapObj(cons, TypeCons);
}

/* Get the CAR or CDR parts of a cons cell */
VyObj Cdr(VyObj cons){
    return ((VyCons*) Obj(cons))->cdr;
}
VyObj Car(VyObj cons){
    return ((VyCons*) Obj(cons))->car;
}

/* Check whether a value is nil. See Cons.h for a description of what nil really is. */
bool IsNil(VyObj obj){
    /* An empty cons cell is nil */
    if(IsType(obj, TypeCons)){
        return IsNone(Car(obj)) && IsNone(Cdr(obj));
    }

    /* Make sure we already have a nil to compare to */
    if(IsNone(nil_symbol))
        nil_symbol = CreateSymbol("nil");
    
    /* The symbol nil is nil */
    return (Obj(obj) == Obj(nil_symbol));
}

/* Assume that this cons cell is a list, return the element at the nth index */
VyObj ListGet(VyCons* list, int index){
    /* If we want the first element, return the car; */
    if(index == 0) return list->car;

    /* Otherwise, we want the n-1th element of the cdr. */
    else return ListGet((VyCons*)Obj(list->cdr), index - 1);
}

/* Find the length of a list with this cons cell at the front */
int ListLen(VyObj list){
    /* Nil ends a list */
    if (IsNil(list)) return 0;

    /* If it isn't nil, move one closer to the end (and increment our return value) */
    else return 1 + ListLen(Cdr(list));
}
