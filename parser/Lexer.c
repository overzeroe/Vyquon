#include "Vyquon.h"

/* Is newline, tab, or space */
inline bool IsWhitespace(char c){
    return c == ' ' || c == '\t' || c == '\n';
}

/* Create and manage token lists */
TokenList* CreateTokenList() {
    Token token = {type: TOKEN_NOTDEF, data: NULL};
    TokenList* tok_list = VyMalloc(sizeof(TokenList));
    tok_list->count = 0;
    tok_list->token = token;
    tok_list->next = NULL;
    return tok_list;
}

/* Append a token to a token list */
TokenList* AppendToken(TokenList* list, Token token){
    /* Create the first token in the list */
    if(list->token.type == TOKEN_NOTDEF){
        list->token = token;
        return list;
    }

    /* Append a new token to the end of the list, return it */
    TokenList* next = VyMalloc(sizeof(TokenList));
    next->count = list->count + 1;
    next->token = token;
    next->next = NULL;
    list->next = next;
    return next;
}

/* Debugging */
void PrintTokens(FILE* out, TokenList* tokens){
    /* Print data */
    char* null_str = "NULL";
    char* str_oparen = "TOKEN_OPAREN";
    char* str_cparen = "TOKEN_CPAREN";
    char* str_symbol = "TOKEN_SYMBOL";
    char* str_quoted = "TOKEN_QUOTED";
    char* str_string = "TOKEN_STRING";

    /* Loop until end of list is reached */
    while(tokens != NULL){
        /* Print type as token type string */
        char* type;
        switch(tokens->token.type){
            case TOKEN_OPAREN: type = str_oparen; break;
            case TOKEN_CPAREN: type = str_cparen; break;
            case TOKEN_SYMBOL: type = str_symbol; break;
            case TOKEN_QUOTED: type = str_quoted; break;
            case TOKEN_STRING: type = str_string; break;
            default: type = null_str;
        }

        /* If no data, just print NULL */
        char* data;
        if(tokens->token.data == NULL) data = null_str;
        else data = tokens->token.data;

        /* Print and advance to next element */
        fprintf(out, "%d: {%s, \"%s\"}\n", tokens->count, type, data);
        tokens = tokens->next;
    }
}

/* Free memory */
void FreeTokens(TokenList* tokens){
    while(tokens != NULL){
        char* data = tokens->token.data;
        if(data) VyFree(data);

        TokenList* next = tokens->next;
        VyFree(tokens);
        tokens = next;
    }
}


/* Read tokens from a text file */
TokenList* LexFile(FILE* file){
    /* Create the list */
    TokenList* tokens = CreateTokenList();
    TokenList* list_start = tokens;
    char next = fgetc(file);

    /* Read until end of file */
    while(!feof(file)){
        /* Use first char to check for start of token */

        Token t;
#define TOKEN(_var, _type, _data) \
        _var.type = _type; \
        _var.data = _data

        int char_count;
        char prev;
        char* str_data = NULL;

        switch(next){
            /* Process single-character tokens */
            case '(':
                TOKEN(t, TOKEN_OPAREN, str_data);
                tokens = AppendToken(tokens, t);
                break;
            case ')':
                TOKEN(t, TOKEN_CPAREN, str_data);
                tokens = AppendToken(tokens, t);
                break;
            case '\'':
                TOKEN(t, TOKEN_QUOTED, str_data);
                tokens = AppendToken(tokens, t);
                break;

            /* Strings start on a quote */
            case '"':
                /* Read to end to count how many chars there are */
                char_count = 0;
                prev = next;
                next = fgetc(file);
                while(true){
                    /* Stop reading on end ", unless escaped */
                    if(next == '"' && prev != '\\') break;
                    if(feof(file)) break;

                    char_count++;
                    prev = next;
                    next = fgetc(file);
                }

                /* Allocate memory for string and read from file */
                char* str_data = VyMalloc(sizeof(char) * char_count);
                fseek(file, -char_count - 1, SEEK_CUR);
                fread(str_data, sizeof(char), char_count, file);

                /* Skip end quote */
                fgetc(file);

                TOKEN(t, TOKEN_STRING, str_data);
                tokens = AppendToken(tokens, t);
                break;

            /* Symbols are anything not dealt with (except whitespace) */
            default:
                if(IsWhitespace(next)) break;

                char_count = 0;

                /* Read until next paren, quote, or whitespace */
                while(true){
                    if(next == ')' || next == '\'' || next == '(' || IsWhitespace(next)) break;
                    if(feof(file)) break;

                    char_count++;

                    next = fgetc(file);
                }

                str_data = VyMalloc(sizeof(char) * char_count);
                fseek(file, -char_count - 1, SEEK_CUR);
                fread(str_data, sizeof(char), char_count, file);
                
                TOKEN(t, TOKEN_SYMBOL, str_data);
                tokens = AppendToken(tokens, t);
                break;
        }

        /* Next char */
        next = fgetc(file);

    }

    return list_start;
}

TokenList* LexString(char* string){
    FILE* file = fmemopen(string, strlen(string), "r");
    TokenList* tokens = LexFile(file);
    fclose(file);
    return tokens;
}

