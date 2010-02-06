#ifndef FUNCTION_H
#define FUNCTION_H

#include "VM.h"
#include "Scope.h"

typedef struct _Param {
    bool optional, keyword, rest;
    VySymbol* name;
    VyObj default_value; /* For optional / keyword / rest arguments */
} Param;

typedef struct _ArgList {
    int num_params;
    Param* params;
} ArgList;

typedef struct _VyFunction {
    ArgList arguments;
    Bytecode* code;
    Scope* live_scope;
} VyFunction;


VyFunction* CreateFunction(ArgList args, Bytecode* code);
ArgList ParseArgList(VyObj);
void BindArguments(ArgList arguments, VyObj values[], int num_args);

#endif /* FUNCTION_H */
