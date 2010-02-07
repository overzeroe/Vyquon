#include "Vyquon.h"


/* Create the object wrapper around actual data */
VyObj WrapObj(void* data, ObjType type){
    VyObj object = {obj: data, type: type};
    return object;
}

/* Retrieve the data or type from an object */
void* Obj(VyObj object){
    return object.obj;
}
ObjType Type(VyObj object){
    return object.type;
}

/* Create an empty object */
VyObj None(){
    VyObj object = {obj: NULL, type: 0};
    return object;
}
/* Check whether the object is empty */
inline bool IsNone(VyObj obj){
    return (obj.type == 0 && obj.obj == NULL);
}

/* Evaluate the truthiness of a value. 'false and nil are false, everything else is true. */
bool IsTrue(VyObj value){
    if(IsNil(value))
        return false;
    if(Type(value) == OBJ_SYM && SymbolEq((VySymbol*) Obj(value), "false"))
        return false;
    return true;
}

#ifdef DEBUG
void PrintObj(FILE*, VyObj);

/* Print a cons cell (as a list) */
void PrintCons(FILE* file, VyCons* cons){
    fprintf(file, "(");

    /* Loop until we reach the end of the list, print each object */
    while(!IsNil(WrapObj(cons, OBJ_CONS))){
        PrintObj(file, cons->car);
        VyObj cdr = cons->cdr;
        
        /* Keep looping */
        if(Type(cdr) == OBJ_CONS)
            cons = (VyCons*) Obj(cdr);

        /* If we've reached the end, but it isn't a nil, use the dot notation */
        else {
            fprintf(file, ". ");
            PrintObj(file, cdr);
            break;
        }
    }
    fprintf(file, "\b) ");
}

/* Write a human-readable version of the object to a file  if we can */
void PrintObj(FILE* file, VyObj obj){
    switch(Type(obj)){
        /* Strings and symbols are just printed */
        case OBJ_STR:
            fprintf(file, "\"%s\" ", ((VyString*) Obj(obj))->str);
            break;
        case OBJ_SYM:
            fprintf(file, "%s ", ((VySymbol*) Obj(obj))->symb);
            break;

        /* Recursively print cons cells as lists */
        case OBJ_CONS:
            PrintCons(file, (VyCons*)Obj(obj));
            break;

        /* Everything else can't really be expressed, just print the type or give an error  */
        case OBJ_FUNC:
            fprintf(file, "Unnamed function lambda at (%p)", Obj(obj));
            break;
        case OBJ_NONE:
            fprintf(file, "Obj-type-NONE. Error!");
            exit(0);
        default:
            printf("Unknown object type.\n");
            exit(0);
    }
}
#endif /* DEBUG */
