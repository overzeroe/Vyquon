#ifndef STRING_H
#define STRING_H

typedef struct _VyString {
    char* str;
} VyString;

VyObj CreateString(char*);

#endif /* STRING_H */
