#include "Vyquon.h"

VyFunction* CreateFunction(ArgList args, Bytecode* code){
    VyFunction* func = VyMalloc(sizeof(VyFunction));
    func->arguments = args;
    func->code.bytecode = code;
    func->native = false;
    return func;
}

VyFunction* CreateNativeFunction(ArgList args, Native native_code){
    VyFunction* func = VyMalloc(sizeof(VyFunction));
    func->arguments = args;
    func->code.native = native_code;
    func->native = true;
    return func;
}

Param ParseParam(VyObj param){
    Param p = {optional: false,
               keyword: false,
               rest: false,
               default_value: None(),
               name: (VySymbol*) Obj(param)};
    return p;
}

ArgList ParseArgList(VyObj list){
    int num = ListLen((VyCons*) Obj(list));
    Param* params = VyMalloc(sizeof(Param) * num);
    int i;
    for(i = 0; i < num; i++)
        params[i] = ParseParam(ListGet((VyCons*) Obj(list), i));

    ArgList args = {num_params: num, params: params};
    return args;
}

void BindArguments(ArgList arguments, VyObj values[], int num_args){
    int i;
    for(i = 0; i < arguments.num_params; i++){
        Param cur = arguments.params[i];
        if(!cur.optional){

            PrintObj(stdout, WrapObj(cur.name, OBJ_SYM));
            printf(" to ");
            PrintObj(stdout, values[i]);
            VariableBind(cur.name, values[i]);
        }
    }
}

