#ifndef CONS_H
#define CONS_H

/* Defines a cons cell structure. A cons cell is the basic unit of structure 
 * in and dialect of Lisp. It's just a fancy word for pair, and the car of it
 * is the first element, and the cdr is the second. List data structures are 
 * built by conjoining cons cells, so (list 1 2 3) would be (1 (2 (3 nil))).
 * The nil at the end signifies the end of a list. Nil is equivalent to
 * a) an empty cons cell b) an empty list c) the symbol 'nil. They are
 * all equal to each other.
 */

/* Cons cell  */
typedef struct _VyCons {
    VyObj car;
    VyObj cdr;
} VyCons;

/* Get an object representing nil. 
 * This may be the symbol 'nil, an empty cons cell, or an empty list.
 * Do not make assumptions about the type of the resulting object;
 * instead, use IsNil() to check whether an object is nil.
 */
VyObj Nil();

/* Return true if an object is equivalent to nil, false otherwise */
bool IsNil(VyObj);


/* Create a cons cell of two objects */
VyObj Cons(VyObj, VyObj);

/* Access the car and cdr of the cons cell */
VyObj Car(VyObj);
VyObj Cdr(VyObj);

/* Assume that the cons cell is actually the head of a list,
 * and use that to find the nth element at the specified index
 */
VyObj ListGet(VyCons*, int index);

/* Assume that the cons cell is actually the head of a list,
 * and use that to find the size of the list (distance to nil)
 */
int ListLen(VyCons*);

#endif /* CONS_H */
