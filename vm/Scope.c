#include "Vyquon.h"

/* A scope contains it's parent (when it was created) and hashes of variable/value pairs */
struct _Scope {
    Scope* parent;

    /* We use the GDSL library for our hashing. 
     * Sadly, their library hash stores void*s, and we want to store VyObj, so we
     * separately store variable values and variable types in two different hashes.
     * This will be changed to be sane eventually.
     */
    gdsl_hash_t var_values;
    gdsl_hash_t type_values;
};

/* Current scope in execution - becomes global when it's first accessed */
Scope* current_scope = NULL;

/* Initialize a scope from a parent (create hashes, whatever) */
Scope* CreateScope(Scope* parent){
    Scope* scope = VyMalloc(sizeof(Scope));
    scope->parent = parent;
    scope->var_values = gdsl_hash_alloc("varvalues", NULL, NULL, NULL, NULL, 5);
    scope->type_values = gdsl_hash_alloc("vartypes", NULL, NULL, NULL, NULL, 5);
    return scope;
}

/* Get current scope */
Scope* CurrentScope(){
    /* If no scope exists, initialize the global one */
    if(!current_scope) current_scope = CreateScope(NULL);

    return current_scope;
}

/* Retrieve a value from the scope */
VyObj VariableValue(VySymbol* symb){
    /* Find the scope in which this was last defined. Try the current scope,
     * but if it isn't there, check the parent scope, and repeat.
     */
    Scope* current = CurrentScope();
    while(!gdsl_hash_search(current->var_values, symb->symb))
        current = current->parent;

    /* Get the value and type from the individual hash tables */
    gdsl_element_t value = gdsl_hash_search(current->var_values, symb->symb);
    gdsl_element_t type = gdsl_hash_search(current->type_values, symb->symb);

    VyObj obj = WrapObj(value, (ObjType)(type));
    return obj;
}

/* Bind a value to a variable in the current scope */
void VariableBind(VySymbol* symb, VyObj obj){
    Scope* current = CurrentScope();
    gdsl_hash_put(current->var_values, Obj(obj), symb->symb);
    gdsl_hash_put(current->type_values, (gdsl_element_t) (Type(obj)), symb->symb);
}

void EnterScope(Scope* scope){
    current_scope = scope;
}
void ClearScope(Scope* scope){
    gdsl_hash_flush(scope->var_values);
    gdsl_hash_flush(scope->type_values);
}
