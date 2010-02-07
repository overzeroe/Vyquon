#ifndef OBJ_H
#define OBJ_H

/* Since Vyquon is a dynamically typed language, we need some way to pass around
 * generically specified objects. At the moment, this is done by passing around 
 * a VyObj struct, which contains the type of object it is, and a pointer to the
 * object itself. This might not be optimal and might not work with our future GC,
 * so this is liable to change.
 */
typedef int ObjType;
typedef struct _VyObj {
    ObjType type;
    void* obj;
} VyObj;

/* Get the object's data pointer */
void* Obj(VyObj);

/* Get the object's type */
ObjType Type(VyObj);

/* Create an object pointing to some data of a certain type */
VyObj WrapObj(void*,ObjType);

/* The None object is a hack for times when we have to pass some object.
 * It means nothing. It can be used to check for things, though, like ends
 * of lists. In general, don't use it.
 */
VyObj None();

/* Check if something is the none object */
bool IsNone(VyObj);

/* Check whether a given object represents truth. We accept two things as
 * false: the symbol 'false and nil. (Nil may have multiple meanings. See Cons.h)
 * Everything that isn't false is true, including 'FALSE and 'False and 0.
 */
bool IsTrue(VyObj);

/* Print a human-readable (if possible) reprentation of an object to a file */
void PrintObj(FILE*, VyObj);

/* Object types */
#define OBJ_CONS 10     /* Cons cell */
#define OBJ_STR  20     /* String */
#define OBJ_SYM  30     /* Symbol */
#define OBJ_NUM  40     /* Number */
#define OBJ_FUNC 50     /* Function */
#define OBJ_NONE 60     /* When we don't care wtf it is */

#endif /* OBJ_H */
