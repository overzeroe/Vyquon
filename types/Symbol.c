#include "Vyquon.h"

gdsl_hash_t symbol_hash = NULL;

/* Create a symbol but not an object */
VySymbol* CreateSymbol_NoObj(char* str){
    /* Initialize hash */
    if(!symbol_hash)
        symbol_hash = gdsl_hash_alloc("symbols", NULL, NULL, NULL, NULL, 1000);


    
    VySymbol* symbol = gdsl_hash_search(symbol_hash, str);
    if(!symbol){
        symbol = VyMalloc(sizeof(VySymbol));

        /* Duplicate string data so we don't end up freeing things twice */
        symbol->symb = strdup(str);
        gdsl_hash_put(symbol_hash, symbol, str);
    } 

    return symbol;
}

/* Create a symbol */
VyObj CreateSymbol(char* str){
    return WrapObj(CreateSymbol_NoObj(str), TypeSymbol);
}

/* Check for equality of two symbols. */
bool SymbolEq(VySymbol* symb, VySymbol* data){
    return (symb == data);
}
