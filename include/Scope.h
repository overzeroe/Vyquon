#ifndef SCOPE_H
#define SCOPE_H

typedef struct _Scope Scope;
Scope* CurrentScope();
Scope* CreateScope(Scope* parent);

void EnterScope(Scope*);

/* Delete all elements in the scope so it can be reused */
void ClearScope(Scope*);

VyObj VariableValue(VySymbol*);
void VariableBind(VySymbol*, VyObj);

#endif /* SCOPE_H */
