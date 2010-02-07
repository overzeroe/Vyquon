#include "Vyquon.h"

/* Create a symbol */
VyObj CreateSymbol(char* str){
    VySymbol* symbol = VyMalloc(sizeof(VySymbol));

    /* Duplicate string data so we don't end up freeing things twice */
    symbol->symb = strdup(str);
    return WrapObj(symbol, OBJ_SYM);
}

/* Check for equality of two symbols. */
bool SymbolEq(VySymbol* symb, char* data){
    return 0 == strcmp(data, symb->symb);
}
