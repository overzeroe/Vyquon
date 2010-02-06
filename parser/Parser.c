#include "Vyquon.h"

VyObj ParseList(TokenList* tokens, TokenList** used){
    if(tokens->token.type == TOKEN_CPAREN){
        *used = tokens->next;
        return Nil();
    }
    else {
        TokenList* last;
        VyObj car = Parse(tokens, &last);
        VyObj cdr = ParseList(last, used);
        return Cons(car, cdr);
    }
}

VyObj ParseQuoted(TokenList* tokens, TokenList** used){
    *used = tokens;
    return Cons(CreateSymbol("quote"), Cons(Parse(tokens, used), Nil()));
}

VyObj Parse(TokenList* tokens, TokenList** used) {
    Token first = tokens->token;
    switch(first.type){
        case TOKEN_OPAREN:
            return ParseList(tokens->next, used);
        case TOKEN_QUOTED:
            return ParseQuoted(tokens->next, used);
        case TOKEN_SYMBOL:
            *used = tokens->next;
            return CreateSymbol(first.data);
        case TOKEN_STRING:
            *used = tokens->next;
            return CreateString(first.data);

        /* Error! */
        default:
            printf("Error. Unexpected symbol type!\n");
            exit(0);
    }

    return Nil();
}
