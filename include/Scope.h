#ifndef SCOPE_H
#define SCOPE_H

/* Scopes provide methods for functions to store variables.
 * Externally, a scope has two things: a parent scope and
 * a hash. The parent scope is the closure scope of the
 * given function, or NULL for the global scope; it is
 * searched for variable bindings if they are not found locally.
 * The hash stores key-value pairs, where the key is the variable
 * name as specified by the symbol, and the value is the variable value.
 * Note that the key is the symbol, not the string; this means that if
 * you have two different symbols with the same string value, they
 * can be bound to different things. This should not regularly happen,
 * though, because symbols are interned when created. (Only one copy exists)
 */

/* Scopes should only be accessed using the provided functions. */
typedef struct _Scope Scope;

/* Get the current scope. The first time this is called, it initializes the global scope. */
Scope* CurrentScope();

/* Create a scope for a function created in the scope of parent */
Scope* CreateScope(Scope* parent);

/* Set a given scope as the current scope */
void EnterScope(Scope*);

/* Delete all elements in the scope so it can be reused */
void ClearScope(Scope*);

/* Get the value of a variable or bind a value to a variable */
VyObj VariableValue(VySymbol*);
void VariableBind(VySymbol*, VyObj);

#endif /* SCOPE_H */
