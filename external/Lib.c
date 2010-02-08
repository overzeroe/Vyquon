#include "Vyquon.h"

/* All libraries will be external, added to the project at link-time */
extern void LoadIO();
extern void LoadMath();
extern void LoadList();
extern void LoadBool();

extern VyType TypeCons, TypeString, TypeSymbol, TypeFloat, TypeInt, TypeFunction;

/* Basic number operations, the rest will be in the math lib */
VyObj CreateFloat(double d){
    VyFloat* flt = VyMalloc(TypeFloat.size);
    flt->val = d;
    return WrapObj(flt, TypeFloat);
}
VyObj CreateInt(int d){
    VyInt* i = VyMalloc(TypeInt.size);
    i->val = d;
    return WrapObj(i, TypeInt);
}

VyObj CreateFloatFromStr(char* str){
    return CreateFloat(atof(str));
}
VyObj CreateIntFromStr(char* str){
    return CreateInt(atoi(str));
}

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

/* Create commonly-used symbols */
extern VyObj SymbolFalse, SymbolIf, SymbolSetvar, SymbolWhile, SymbolNil, SymbolQuote, SymbolFn;
void CreateSymbols(){
    SymbolFn        = CreateSymbol("fn");
    SymbolFalse     = CreateSymbol("false");
    SymbolIf        = CreateSymbol("if");
    SymbolSetvar    = CreateSymbol("setvar");
    SymbolWhile     = CreateSymbol("while");
    SymbolNil       = CreateSymbol("nil");
    SymbolQuote     = CreateSymbol("quote");
}

/* Load the core library */
void LoadCoreLibrary(){
    CreateTypes();

    CreateSymbols();

    LoadMath();
    LoadBool();
    LoadList();
    LoadIO();
}

