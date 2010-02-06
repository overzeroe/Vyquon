#ifndef OBJ_H
#define OBJ_H

typedef struct _VyObj {
    ObjType type;
    void* obj;
} VyObj;

void* Obj(VyObj);
ObjType Type(VyObj);

VyObj WrapObj(void*,ObjType);

VyObj None();
#define IsNone(x) (x.type == 0 && x.obj == NULL)

bool IsTrue(VyObj);

void PrintObj(FILE*, VyObj);

/* Object types */
#define OBJ_CONS 10
#define OBJ_STR  20
#define OBJ_SYM  30 
#define OBJ_NUM  40
#define OBJ_FUNC 50
#define OBJ_NONE 60 /* When we don't care wtf it is */

#endif /* OBJ_H */
