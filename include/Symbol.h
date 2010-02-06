#ifndef SYMBOL_H
#define SYMBOL_H

/* Potentially replace with integer index ID instead of char*, since these should really be interned from the start */
typedef struct _VySymbol {
    char* symb;
} VySymbol;

VyObj CreateSymbol(char*);

bool SymbolEq(VySymbol*, char*);

#endif /* SYMBOL_H */
