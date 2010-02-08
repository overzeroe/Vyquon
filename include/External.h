#ifndef EXTERNAL_H
#define EXTERNAL_H

/* Register a new native function in the current scope. It will be
 * bound to the name specified by the first argument, and argument
 * checking will be done in accordance with the argument list given.
 * the argument list should be one list of the form (arg1 arg2 etc),
 * as if it were the argument list of an interpreted function.
 * The last argument to this function is a function pointer to a function
 * which takes 1) an array of objects, VyObj*, 2) an integer saying how many
 * elements are in that array, 3) returns an object type VyObj.
 */
void NewFunction(char* name, char* argument_list, Native function);

/* The true and false boolean values */
VyObj TrueObj();
VyObj FalseObj();

/* Check whether a given object represents truth. We accept two things as
 * false: the symbol 'false and nil. (Nil may have multiple meanings. See Cons.h)
 * Everything that isn't false is true, including 'FALSE and 'False and 0.
 */
bool IsTrue(VyObj);
bool IsFalse(VyObj);

/* Provide external libraries with a convenient way to access things inside objects */
#define UNWRAP(__x, __type) ((__type*) Obj(__x))


/* Numeric structures are necessary */
typedef struct _VyInt {
    int val;
} VyInt;
typedef struct _VyFloat {
    double val;
} VyFloat;

VyObj CreateInt(int);
VyObj CreateFloat(double);

/* Check for numeric equality */
bool NumEq(VyObj, VyObj);

#endif /* EXTERNAL_H */
