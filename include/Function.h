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

typedef VyObj (*Native)(VyObj*, int); 
typedef struct _VyFunction {
    ArgList arguments;
    union {
        Bytecode* bytecode;
        Native native;
    } code;
    bool native;
    Scope* live_scope;
} VyFunction;


VyFunction* CreateFunction(ArgList args, Bytecode* code);
VyFunction* CreateNativeFunction(ArgList args, Native native_code);
ArgList ParseArgList(VyObj);
void BindArguments(ArgList arguments, VyObj values[], int num_args);

#endif /* FUNCTION_H */
