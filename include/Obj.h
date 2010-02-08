#ifndef OBJ_H
#define OBJ_H

/* Since Vyquon is a dynamically typed language, we need some way to pass around
 * generically specified objects. At the moment, this is done by passing around 
 * a VyObj struct, which contains the type of object it is, and a pointer to the
 * object itself. This might not be optimal and might not work with our future GC,
 * so this is liable to change.
 */
struct _VySymbol;
typedef struct _VyType {
    int size;
    struct _VySymbol* name;   
} VyType;

typedef struct _VyObj {
    VyType type;
    void* obj;
} VyObj;

/* Get the object's data pointer */
void* Obj(VyObj);

/* Get the object's type */
VyType Type(VyObj);

/* Check whether an object is of a given type */
bool IsType(VyObj, VyType);

/* Create a new type of object. A type is defined by it's name (a symbol)
 * and by the amount of memory it takes up (the size).
 */
VyType CreateType(int size, struct _VySymbol* name);

/* Create an object pointing to some data of a certain type */
VyObj WrapObj(void*,VyType);

/* Compare two objects */
bool ObjEq(VyObj, VyObj);

/* The None object is a hack for times when we have to pass some object.
 * It means nothing. It can be used to check for things, though, like ends
 * of lists. In general, don't use it.
 */
VyObj None();

/* Check if something is the none object */
bool IsNone(VyObj);

/* Print a human-readable (if possible) reprentation of an object to a file */
void PrintObj(FILE*, VyObj);

/* Global types */
VyType TypeCons, TypeString, TypeSymbol, TypeFloat, TypeInt, TypeFunction, TypeNone;

#endif /* OBJ_H */
