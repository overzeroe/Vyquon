#include "Vyquon.h"

/* Finish the parsing of a list. This function is called after an OPAREN token
 * is found, so the next CPAREN should close the list. 
 */
VyObj ParseList(TokenList* tokens, TokenList** used){
    /* If there is a CPAREN, close the list */
    if(tokens->token.type == TOKEN_CPAREN){
        /* Store end point */
        *used = tokens->next;

        /* Return Nil because lists are cons cells which end with nil */
        return Nil();
    }
    else {
        /* If we're not at the end of the list, parse the current element as the car,
         * and then parse the rest as the remainder of the list (the cdr)
         */
        TokenList* last;
        VyObj car = Parse(tokens, &last);
        VyObj cdr = ParseList(last, used);

        /* Return a cons cell; these are formed into lists */
        return Cons(car, cdr);
    }
}

/* Create a quoted element. 'X expands into (quote X) */
VyObj ParseQuoted(TokenList* tokens, TokenList** used){
    *used = tokens;
    /* Returns (cons 'quote (cons ____ nil)), where __ is whatever is following the ' */
    return Cons(CreateSymbol("quote"), Cons(Parse(tokens, used), Nil()));
}

extern VyObj CreateFloatFromStr(char*);
extern VyObj CreateIntFromStr(char*);

/* The recursive front end to the parser */
VyObj Parse(TokenList* tokens, TokenList** used) {
    /* Determine what to do based on the first token */
    Token first = tokens->token;
    switch(first.type){
        /* Parenthesis means it's a list */
        case TOKEN_OPAREN:
            return ParseList(tokens->next, used);

        /* ' means it's a quoted form */
        case TOKEN_QUOTED:
            return ParseQuoted(tokens->next, used);

        /* Symbols, strings, and numbers are just converted into their object counterparts */
        case TOKEN_SYMBOL:
            *used = tokens->next;
            return CreateSymbol(first.data);
        case TOKEN_STRING:
            *used = tokens->next;
            return CreateString(first.data);
        case TOKEN_NUMFLT:
            *used = tokens->next;
            return CreateFloatFromStr(first.data);
        case TOKEN_NUMINT:
            *used = tokens->next;
            return CreateIntFromStr(first.data);

        /* Undefined tokens only occur if there are no tokens */
        case TOKEN_NOTDEF:
            fprintf(stderr, "Error: No code in the file.\n");
            exit(0);

        /* Error! We shouldn't have other token types. */
        default:
            fprintf(stderr, "Error: Unexpected token type\n");
            exit(0);
    }

    /* It never gets here */
    return Nil();
}
