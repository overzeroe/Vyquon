#include "Vyquon.h"

/* A parameter to a function. Contains flags for whether
 * it is an optional parameter or
 * a rest parameter. If the rest flag is true,
 * then the optional flag should also be true. Also contains
 * the default value, which is used for optional 
 * arguments. The default value is always '() for rest arguments
 * so the one provided in the param is not used.  */
struct _Param {
    bool optional, rest;
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
Param ParseParam(VyObj param, bool opt, bool rest){
    VyObj default_val = None();
    if(opt){
        if(IsType(param, TypeCons)){
            VyObj name = ListGet(UNWRAP(param, VyCons), 0);

            default_val = ListGet(UNWRAP(param, VyCons), 0);
            param = name;
        }
    }

    Param p = {optional: opt,
        rest: rest,
        default_value: default_val,
        name: UNWRAP(param, VySymbol)};
    return p;
}

/* Find how many actual parameters there are */
int CountParams(VyObj list){
    int count = 0;
    while(!IsNil(list)){
        if(!ObjEq(Car(list), CreateSymbol("?")) && 
           !ObjEq(Car(list), CreateSymbol("..")))
                count++;

        list = Cdr(list);
    }

    return count;
}

/* Given the argument list, parse it into ArgList format. */
ArgList ParseArgList(VyObj list){
    int num = CountParams(list);
    Param* params = VyMalloc(sizeof(Param) * num);
    int i;

    /* Each element of the list is either a parameter or an argument mode setting symbol...
     * Check if it sets the argument type (?, ~, or ..), and if it doesn't, parse it as a parameter 
     */
    VyObj opt_arg_set = CreateSymbol("?");
    VyObj rest_arg_set = CreateSymbol("..");

    bool opt, rest;
    opt = rest = false;

    int param_num = 0;
    int arg_list_len = ListLen(list);
    for(i = 0; i < arg_list_len; i++){
        VyObj next = ListGet(UNWRAP(list, VyCons), i);

        if(ObjEq(next, opt_arg_set)){
            opt = true;
        }
        else if(ObjEq(next, rest_arg_set)){
            opt = rest = true;
        }
        else {
            params[param_num] = ParseParam(next, opt, rest);
            param_num++;
        }

    }

    ArgList args = {num_params: num, params: params};
    return args;
}

/* Bind values to variables in the current scope according to the format of the argument list */
void BindArguments(ArgList arguments, VyObj values[], int num_args){
    int i;
    for(i = 0; i < num_args; i++){
        Param cur = arguments.params[i];
        /* Normal arguments can just be bound right away */
        if(!cur.rest){
            VariableBind(cur.name, values[i]);
        } else {
            VyObj list = Nil();
            int j;
            for(j = num_args - 1; j >= i; j--)
                list = Cons(values[j], list);
            VariableBind(cur.name, list);
            i = num_args;
        }
    }

    /* Finish filling in arguments that weren't passed */
    for(; i < arguments.num_params; i++){
        Param cur = arguments.params[i];
        if(cur.rest)
            VariableBind(cur.name, Nil());
        else {
            if(IsNone(cur.default_value))
                cur.default_value = Nil();
            VariableBind(cur.name, cur.default_value);
        }
    }
}

