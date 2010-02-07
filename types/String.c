#include "Vyquon.h"

/* Create a string */
VyObj CreateString(char* str){
    VyString* string = VyMalloc(sizeof(VyString));

    /* We don't want the data from tokens to be stored, since we try to
     * free all tokens no matter what; thus, duplicate the data.  */
    string->str = strdup(str);
    return WrapObj(string, OBJ_STR);
}
