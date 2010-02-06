#ifndef CONS_H
#define CONS_H

typedef struct _VyCons {
    VyObj car;
    VyObj cdr;
} VyCons;

VyObj Nil();
VyObj Cons(VyObj, VyObj);
VyObj Car(VyCons*);
VyObj Cdr(VyCons*);
bool IsNil(VyObj);

VyObj ListGet(VyCons*, int index);
int ListLen(VyCons*);

#endif /* CONS_H */
