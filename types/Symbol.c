#include "Vyquon.h"

GHashTable* symbol_hash = NULL;

/* Create a symbol but not an object */
VySymbol* CreateSymbol_NoObj(char* str){
    /* Initialize hash */
    if(!symbol_hash)
        symbol_hash = g_hash_table_new(g_str_hash, g_str_equal);

    
    VySymbol* symbol = g_hash_table_lookup(symbol_hash, str);
    if(!symbol){
        symbol = VyMalloc(sizeof(VySymbol));

        /* Duplicate string data so we don't end up freeing things twice */
        symbol->symb = strdup(str);
        g_hash_table_insert(symbol_hash, str, symbol);
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

/* Delete the GHashTable* that holds the symbols */
void DeleteInternedSymbols(){
    g_hash_table_unref(symbol_hash);
}
