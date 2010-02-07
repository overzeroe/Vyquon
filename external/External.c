#include "Vyquon.h"

/* Register a new function with the runtime */
void NewFunction(char* name, char* argument_list, Native function){
    /* Create a symbol for the name */
    VySymbol* symbol_name = (VySymbol*) Obj(CreateSymbol(name));

    /* Parse the argument list */
    TokenList* tokens = LexString(argument_list);
    VyObj parsed = Parse(tokens, &tokens);
    ArgList arguments = ParseArgList(parsed);
    FreeTokens(tokens);

    /* Create the function object */
    VyFunction* func = CreateNativeFunction(arguments, function);
    VyObj func_obj = WrapObj(func, OBJ_FUNC);

    /* Bind it to name in the current (probably global) scope */
    VariableBind(symbol_name, func_obj);
}
