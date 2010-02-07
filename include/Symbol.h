#ifndef SYMBOL_H
#define SYMBOL_H

/* Symbols represent names of things in code, such as functions, variables, etc.
 * For this reason, they should be interned - namely, the symbol of "FOO" should
 * always be referring to the same FOO. Only one copy of a symbol should exist.
 * However, this may be not the case sometimes, but if it isn't, this runtime
 * should treat the two different instances of the symbol as if they were
 * entirely distinct symbols in the first place.
 */

typedef struct _VySymbol {
    char* symb;
} VySymbol;

/* Create a symbol */
VyObj CreateSymbol(char*);

/* Create a symbol but don't return it as an object (rarely needed) */
VySymbol* CreateSymbol_NoObj(char*);

/* Check for symbol equality... this should not be comparing a symbol to a char* (for reasons mentioned above) */
bool SymbolEq(VySymbol*, char*);

#endif /* SYMBOL_H */
