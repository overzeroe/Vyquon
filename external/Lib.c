#include "Vyquon.h"

/* All libraries will be external, added to the project at link-time */
extern void LoadIO();
extern void LoadMath();
extern void LoadList();

extern VyType TypeCons, TypeString, TypeSymbol, TypeFloat, TypeInt, TypeFunction;

typedef struct _VyInt {

} VyInt;
typedef struct _VyFloat {

} VyFloat;

/* Create types */
void CreateTypes(){
    TypeNone = CreateType(0, NULL);          
    TypeCons    = CreateType(sizeof(VyCons), CreateSymbol_NoObj("cons"));
    TypeString  = CreateType(sizeof(VyString), CreateSymbol_NoObj("string"));
    TypeSymbol  = CreateType(sizeof(VySymbol), CreateSymbol_NoObj("symbol"));
    TypeFloat   = CreateType(sizeof(VyFloat), CreateSymbol_NoObj("float"));
    TypeInt     = CreateType(sizeof(VyInt), CreateSymbol_NoObj("int"));
    TypeFunction = CreateType(sizeof(VyFunction), CreateSymbol_NoObj("function"));

}

/* Load the core library */
void LoadCoreLibrary(){
    CreateTypes();

    LoadMath();
    LoadList();
    LoadIO();
}
