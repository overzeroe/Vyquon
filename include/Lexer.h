#ifndef LEXER_H
#define LEXER_H

/* Memory rules: Lex*() functions allocate memory for token lists; 
       use LexFree(TokenList*) to free all memory appropriately. 
*/

typedef struct _Token {
    /* Type of token (parenthesis, brace, etc) */
    char type;

    /* Any associated data; NULL if no data is needed */
    char* data;
} Token;

/* Token types */
#define TOKEN_OPAREN 10
#define TOKEN_CPAREN 20
#define TOKEN_SYMBOL 30
#define TOKEN_QUOTED 40
#define TOKEN_STRING 50
#define TOKEN_NOTDEF 60

struct _TokenList;
typedef struct _TokenList {
    /* Language information */
    Token token;

    /* Number of token in the list */
    int count;

    /* Next token (NULL if end of list) */
    struct _TokenList* next;

} TokenList;

/* Read tokens from a file */
TokenList* LexFile(FILE* infile);

/* Print tokens (debugging) */
void PrintTokens(FILE* outfile, TokenList* tokens);

/* Free memory used by tokens */
void FreeTokens(TokenList* tokens);

#endif /* LEXER_H */
