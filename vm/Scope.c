#include "Vyquon.h"

struct _Scope {
    Scope* parent;

    gdsl_hash_t var_values;
    gdsl_hash_t type_values;
};

Scope* current_scope = NULL;

Scope* CreateScope(Scope* parent){
    Scope* scope = VyMalloc(sizeof(Scope));
    scope->parent = parent;
    scope->var_values = gdsl_hash_alloc("varvalues", NULL, NULL, NULL, NULL, 5);
    scope->type_values = gdsl_hash_alloc("vartypes", NULL, NULL, NULL, NULL, 5);
    return scope;
}

Scope* CurrentScope(){
    if(!current_scope) current_scope = CreateScope(NULL);
    return current_scope;
}

VyObj VariableValue(VySymbol* symb){
    Scope* current = CurrentScope();
    gdsl_element_t value = gdsl_hash_search(current->var_values, symb->symb);
    gdsl_element_t type = gdsl_hash_search(current->type_values, symb->symb);

    VyObj obj = WrapObj(value, (ObjType)(type));
    return obj;
}

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
