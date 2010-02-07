#include "Vyquon.h"


/* Create the object wrapper around actual data */
VyObj WrapObj(void* data, VyType type){
    VyObj object = {obj: data, type: type};
    return object;
}

/* Retrieve the data or type from an object */
void* Obj(VyObj object){
    return object.obj;
}
VyType Type(VyObj object){
    return object.type;
}

bool IsType(VyObj obj, VyType type){
    return (Type(obj).name == type.name);
}

/* Create another type */
VyType CreateType(int size, VySymbol* type_name){
    VyType new_type = {size: size, name: type_name};
    return new_type;
}


/* Create an empty object */
VyObj None(){
    VyObj object = {obj: NULL, type: TypeNone };
    return object;
}
/* Check whether the object is empty */
inline bool IsNone(VyObj obj){
    return IsType(obj, TypeNone);
}

/* Evaluate the truthiness of a value. 'false and nil are false, everything else is true. */
bool IsTrue(VyObj value){
    if(IsNil(value))
        return false;
    if(IsType(value, TypeSymbol) && SymbolEq((VySymbol*) Obj(value), "false"))
        return false;
    return true;
}

#ifdef DEBUG
void PrintObj(FILE*, VyObj);

/* Print a cons cell (as a list) */
void PrintCons(FILE* file, VyCons* cons){
    fprintf(file, "(");

    /* Loop until we reach the end of the list, print each object */
    while(!IsNil(WrapObj(cons, TypeCons))){
        PrintObj(file, cons->car);
        VyObj cdr = cons->cdr;
        
        /* Keep looping */
        if(IsType(cdr, TypeCons))
            cons = (VyCons*) Obj(cdr);

        /* Break on nil */
        else if(IsNil(cdr))
            break;

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
    /* Strings and symbols are just printed */
    if(IsType(obj, TypeString))
        fprintf(file, "\"%s\" ", ((VyString*) Obj(obj))->str);
    else if(IsType(obj, TypeSymbol))
        fprintf(file, "%s ", ((VySymbol*) Obj(obj))->symb);

    /* Recursively print cons cells as lists */
    else if(IsType(obj, TypeCons))
        PrintCons(file, (VyCons*)Obj(obj));

    /* Everything else can't really be expressed, just print the type or give an error  */
    else if(IsType(obj, TypeFunction))
        fprintf(file, "Unnamed function lambda at (%p)", Obj(obj));
    else if(IsType(obj, TypeNone)){
        fprintf(file, "Obj-type-NONE. Error!");
        exit(0);
    }
    else {
        printf("Unknown object type.\n");
        exit(0);
    }
}
#endif /* DEBUG */
