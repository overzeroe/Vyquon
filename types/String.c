#include "Vyquon.h"

VyObj CreateString(char* str){
    VyString* string = VyMalloc(sizeof(VyString));
    string->str = strdup(str);
    return WrapObj(string, OBJ_STR);
}
