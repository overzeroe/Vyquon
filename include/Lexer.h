#ifndef LEXER_H
#define LEXER_H

/* Define a token structure. It contains the type of token
 * (paren, close paren, symbol, etc) and any associated data.
 * Most of the token types do not require associated data, so
 * that field will be NULL for them; symbols and strings use the 
 * data field for storing the string.
 */
typedef struct _Token {
    char type;
    char* data;
} Token;

/* Token types */
#define TOKEN_OPAREN 10     /* Opening parenthesis */
#define TOKEN_CPAREN 20     /* Closing parenthesis */
#define TOKEN_SYMBOL 30     /* Symbol */
#define TOKEN_QUOTED 40     /* Quote mark '*/
#define TOKEN_STRING 50     /* String, delimited by quotes "" */
#define TOKEN_NOTDEF 60     /* Nothing. If this is encountered, it is an error.
                             * may be used for things like start/end of token lists. */


/* Define a linked list of tokens. Contains the token data,
 * the number of this token in the list, and a link to the
 * next token in the linked list. The link will be NULL
 * for the last token in the list.
 */
typedef struct _TokenList TokenList;
struct _TokenList {
    Token token;
    int count;
    TokenList* next;

};

/* Read tokens from a file or string */
TokenList* LexFile(FILE* infile);
TokenList* LexString(char* instring);

/* Print tokens to a file (debugging) */
void PrintTokens(FILE* outfile, TokenList* tokens);

/* Free memory used by tokens */
void FreeTokens(TokenList* tokens);

#endif /* LEXER_H */
