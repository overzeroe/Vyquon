#include "Vyquon.h"

VyObj CreateSymbol(char* str){
    VySymbol* symbol = VyMalloc(sizeof(VySymbol));
    symbol->symb = strdup(str);
    return WrapObj(symbol, OBJ_SYM);
}

bool SymbolEq(VySymbol* symb, char* data){
    return 0 == strcmp(data, symb->symb);
}
