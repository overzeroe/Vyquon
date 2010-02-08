#include "Vyquon.h"

/* Is this character a newline, tab, or space? */
inline bool IsWhitespace(char c){
    return c == ' ' || c == '\t' || c == '\n';
}

/* Is it numeric */
inline bool IsNumeric(char c){
    return '0' <= c && c <= '9';
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


#ifdef DEBUG
/* Print tokens in some human-readable way */
void PrintTokens(FILE* out, TokenList* tokens){
    /* Print data */
    char* null_str = "NULL";
    char* str_oparen = "TOKEN_OPAREN";
    char* str_cparen = "TOKEN_CPAREN";
    char* str_symbol = "TOKEN_SYMBOL";
    char* str_quoted = "TOKEN_QUOTED";
    char* str_string = "TOKEN_STRING";
    char* str_numflt = "TOKEN_NUMFLT";
    char* str_numint = "TOKEN_NUMINT";

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
            case TOKEN_NUMINT: type = str_numint; break;
            case TOKEN_NUMFLT: type = str_numflt; break;
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
#endif /* DEBUG */

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

/* Decide whether it is a symbol, int, or float */
int FindTokenType(char* data){
    /* Parse the first number */
    char* end_ptr;
    double val = strtod(data, &end_ptr);

    /* If this token is a number, the entire thing will be parsed up to the null terminator  */
    if(end_ptr[0] != '\0')
        return TOKEN_SYMBOL;
    else {
        /* Check whether it's int or float by comparing values */
        int int_val = atoi(data);
        if(int_val == val){
            /* Even if values are the same, it could be something like 5.0, which should be a double. 
             * Thus we check for periods (radices) to make sure the user isn't trying to make a float */
            int i;
            for(i = 0; i < strlen(data); i++)
                if(data[i] == '.')
                    return TOKEN_NUMFLT;

            return TOKEN_NUMINT;
        }
        else {
            return TOKEN_NUMFLT;
        }
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
            /* Skip comments */
            case ';':
                while(next != '\n'){
                    next = fgetc(file);
                }
                break;
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
                char* str_data = VyMalloc(sizeof(char) * (char_count + 1));
                fseek(file, -char_count - 1, SEEK_CUR);
                fread(str_data, sizeof(char), char_count, file);

                /* End the string! */
                str_data[char_count] = '\0';

                /* Skip end quote */
                fgetc(file);

                TOKEN(t, TOKEN_STRING, str_data);
                tokens = AppendToken(tokens, t);
                break;

            /* Symbols are anything not dealt with (except whitespace) */
            /* Additionally, deal with numbers, which are just like symbols except have numeric characters */
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

                /* Read the token from a file, but remember to allocate an extra byte for the null terminator */
                str_data = VyMalloc(sizeof(char) * (char_count + 1));
                fseek(file, -char_count - 1, SEEK_CUR);
                fread(str_data, sizeof(char), char_count - 0, file);

                str_data[char_count] = '\0';

                
                /* Now that we've separated the token out, decide whether it really is a symbol or
                 * whether it is actually an integer or floating point number */
                TOKEN(t, FindTokenType(str_data), str_data);
                tokens = AppendToken(tokens, t);
                break;
        }

        /* Next char */
        next = fgetc(file);

    }

    return list_start;
}

/* Read tokens from a string */
TokenList* LexString(char* string){
    /* Open a file stream to the memory in which the string is stored */
    FILE* file = fmemopen(string, strlen(string), "r");
    TokenList* tokens = LexFile(file);

    fclose(file);
    return tokens;
}

