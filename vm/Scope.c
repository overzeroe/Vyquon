#include "Vyquon.h"

/* A scope contains it's parent (when it was created) and hashes of variable/value pairs */
struct _Scope {
    Scope* parent;

    /* We use the Glib hash library for our hashing. 
     * Sadly, their library hash stores void*s, and we want to store VyObj, so we
     * separately store variable values and variable types in two different hashes.
     * This will be changed to be sane eventually.
     */
    GHashTable* var_values;
    GHashTable* type_values;
    GHashTable* size_values;
};

/* Current scope in execution - becomes global when it's first accessed */
Scope* current_scope = NULL;

/* Initialize a scope from a parent (create hashes, whatever) */
Scope* CreateScope(Scope* parent){
    Scope* scope = VyMalloc(sizeof(Scope));
    scope->parent = parent;
    scope->var_values = g_hash_table_new(g_str_hash, g_str_equal);
    scope->type_values = g_hash_table_new(g_str_hash, g_str_equal);
    scope->size_values = g_hash_table_new(g_str_hash, g_str_equal);
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
    while(!g_hash_table_lookup(current->var_values, symb->symb)){
        current = current->parent;
        if(!current){
            printf("Failed to find: %s\n", symb->symb);
            fflush(stdout);
        }
    }

    /* Get the value and type from the individual hash tables */
    gpointer value = g_hash_table_lookup(current->var_values, symb->symb);
    gpointer name = g_hash_table_lookup(current->type_values, symb->symb);
    gpointer size = g_hash_table_lookup(current->size_values, symb->symb);


    VyType type = {size: (int) size, name: name};
    VyObj obj = WrapObj(value, type);
    return obj;
}

/* Bind a value to a variable in the current scope */
void VariableBind(VySymbol* symb, VyObj obj){
    Scope* current = CurrentScope();
    g_hash_table_insert(current->var_values, symb->symb, Obj(obj));
    g_hash_table_insert(current->type_values, symb->symb, (gpointer) (Type(obj).name));
    g_hash_table_insert(current->size_values, symb->symb, (gpointer) (Type(obj).size));
}

void EnterScope(Scope* scope){
    current_scope = scope;
}
void DeleteScope(Scope* scope){
    g_hash_table_destroy(scope->var_values);
    g_hash_table_destroy(scope->type_values);
    g_hash_table_destroy(scope->size_values);
    VyFree(scope);
}
