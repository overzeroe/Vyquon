#ifndef FUNCTION_H
#define FUNCTION_H

/* Must include due to circular references */
#include "VM.h"
#include "Scope.h"

/* The parameters are taken care of internally */
typedef struct _Param Param;

/* An argument list, consists of a list of parameters.
 * Contains: number of parameters, pointer to first parameter.
 * Note that the number of parameters may be unequal to the number
 * of arguments, due to the existence of optional, keyword, and
 * rest parameters. Parameter lists should not be manipulated externally;
 * that may result in undefined behaviour.
 */
typedef struct _ArgList {
    int num_params;
    Param* params;
} ArgList;


/* A native function must take a list of objects and an int saying how many there are,
 * and it must return an object. (Every expression must result in an object.)
 * Use the external interface NewFunction() to add native functions to the runtime.
 */
typedef VyObj (*Native)(VyObj*, int); 

/* A function structure. Contains: an argument list defining how arguments should
 * be accepted; a code pointer, either to bytecode (for interpreted functions) or to
 * a function pointer (for native functions), and a boolean to determine whether the
 * function is native or interpreted. Also contains the function's live scope.
 * The live scope is used to store variable bindings during the functions execution.
 * To avoid excessive memory allocation/fragmentation, it is reused every time. Thus,
 * after the function finishes, the live scope is cleared for reuse next time. It's
 * parent is the scope in which the function was created (thus making the function
 * a closure over that scope). Use CreateFunction() and CreateNativeFunction() to
 * create function objects.
 */
typedef struct _VyFunction {
    ArgList arguments;
    union {
        Bytecode* bytecode;
        Native native;
    } code;
    bool native;
    Scope* creation_scope;
} VyFunction;


/* Create function objects from the argument list specification and either bytecode or a native function pointer. */
VyFunction* CreateFunction(ArgList args, Bytecode* code);
VyFunction* CreateNativeFunction(ArgList args, Native native_code);

/* Create an argument list from an object, which should be a list of form (arg1 arg2 ...). */
ArgList ParseArgList(VyObj);

/* Bind the arguments passed to names, depending on the argument list. This
 * function will deal with things like keyword arguments, optional arguments,
 * putting extra arguments into rest arguments, etc.
 */
void BindArguments(ArgList arguments, VyObj values[], int num_args);

#endif /* FUNCTION_H */
