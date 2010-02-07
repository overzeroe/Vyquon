#include "Vyquon.h"

/* This can return an empty list or the symbol 'nil */
VyObj Nil(){
    /* Create a cons cell where both of the values are nonexisten */
    VyCons* cons = VyMalloc(sizeof(VyCons));
    cons->car = None();
    cons->cdr = None();
    return WrapObj(cons, OBJ_CONS);
}

/* Conjoin two objects */
VyObj Cons(VyObj car, VyObj cdr){
    VyCons* cons = VyMalloc(sizeof(VyCons));
    cons->car = car;
    cons->cdr = cdr;
    return WrapObj(cons, OBJ_CONS);
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
    if(Type(obj) == OBJ_CONS){
        return IsNone(Car(obj)) && IsNone(Cdr(obj));
    }
    
    /* The symbol nil is nil */
    if(Type(obj) == OBJ_SYM){
        return SymbolEq((VySymbol*) Obj(obj), "nil");
    }

    /* Everything else isn't nil */
    return false;
}

/* Assume that this cons cell is a list, return the element at the nth index */
VyObj ListGet(VyCons* list, int index){
    /* If we want the first element, return the car; */
    if(index == 0) return list->car;

    /* Otherwise, we want the n-1th element of the cdr. */
    else return ListGet((VyCons*)Obj(list->cdr), index - 1);
}

/* Find the length of a list with this cons cell at the front */
int ListLen(VyCons* list){
    /* Nil ends a list */
    if (IsNone(list->car) && IsNone(list->cdr)) return 0;

    /* If it isn't nil, move one closer to the end (and increment our return value) */
    else return 1 + ListLen((VyCons*) Obj(list->cdr));
}
