#include "Vyquon.h"

/* A parameter to a function. Contains flags for whether
 * it is an optional parameter, a keyword parameter, or
 * a rest parameter. If the keyword or rest flags are true,
 * then the optional flag should also be true. Also contains
 * the default value, which is used for optional and keyword
 * arguments. The default value is always '() for rest arguments
 * so the one provided in the param is not used.  */
struct _Param {
    bool optional, keyword, rest;
    VySymbol* name;
    VyObj default_value; /* Ignored for normal and rest arguments */
};

/* Create a function object (interpreted) */
VyFunction* CreateFunction(ArgList args, Bytecode* code){
    VyFunction* func = VyMalloc(sizeof(VyFunction));
    func->arguments = args;
    func->code.bytecode = code;
    func->native = false;
    return func;
}

/* Create a function object (native code) */
VyFunction* CreateNativeFunction(ArgList args, Native native_code){
    VyFunction* func = VyMalloc(sizeof(VyFunction));
    func->arguments = args;
    func->code.native = native_code;
    func->native = true;
    return func;
}

/* Parse a parameter. Currently only supports normal ones. */
Param ParseParam(VyObj param){
    Param p = {optional: false,
               keyword: false,
               rest: false,
               default_value: None(),
               name: (VySymbol*) Obj(param)};
    return p;
}

/* Given the argument list, parse it into ArgList format. */
ArgList ParseArgList(VyObj list){
    int num = ListLen(list);
    Param* params = VyMalloc(sizeof(Param) * num);
    int i;

    /* Each element of the list is a parameter, parse them individually */
    for(i = 0; i < num; i++)
        params[i] = ParseParam(ListGet((VyCons*) Obj(list), i));

    ArgList args = {num_params: num, params: params};
    return args;
}

/* Bind values to variables in the current scope according to the format of the argument list */
void BindArguments(ArgList arguments, VyObj values[], int num_args){
    int i;
    for(i = 0; i < arguments.num_params; i++){
        Param cur = arguments.params[i];
        /* Normal arguments can just be bound right away */
        if(!cur.optional){
            VariableBind(cur.name, values[i]);
        }
    }
}

