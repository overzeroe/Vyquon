#include "Vyquon.h"

void* Obj(VyObj object){
    return object.obj;
}

ObjType Type(VyObj object){
    return object.type;
}

VyObj WrapObj(void* data, ObjType type){
    VyObj object = {obj: data, type: type};
    return object;
}

VyObj None(){
    VyObj object = {obj: NULL, type: 0};
    return object;
}

bool IsTrue(VyObj value){
    if(IsNil(value))
        return false;
    if(Type(value) == OBJ_SYM && SymbolEq((VySymbol*) Obj(value), "false"))
        return false;
    return true;
}

void PrintObj(FILE*, VyObj);
void PrintCons(FILE* file, VyCons* cons){
    fprintf(file, "(");
    while(!IsNil(WrapObj(cons, OBJ_CONS))){
        PrintObj(file, cons->car);
        VyObj cdr = cons->cdr;
        if(Type(cdr) == OBJ_CONS)
            cons = (VyCons*) Obj(cdr);
        else {
            fprintf(file, ". ");
            PrintObj(file, cdr);
            break;
        }
    }
    fprintf(file, "\b) ");
}

void PrintObj(FILE* file, VyObj obj){
    switch(Type(obj)){
        case OBJ_STR:
            fprintf(file, "\"%s\" ", ((VyString*) Obj(obj))->str);
            break;
        case OBJ_SYM:
            fprintf(file, "%s ", ((VySymbol*) Obj(obj))->symb);
            break;
        case OBJ_CONS:
            PrintCons(file, (VyCons*)Obj(obj));
            break;
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

