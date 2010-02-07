#ifndef STRING_H
#define STRING_H

/* Strings are a basic data type which just contain a sequence of characters.
 * Unlike symbols, you can have two distinct strings with the same content.
 */
typedef struct _VyString {
    char* str;
} VyString;

/* Creates a string from a sequence of characters */
VyObj CreateString(char*);

#endif /* STRING_H */
