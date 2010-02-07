#ifndef PARSER_H
#define PARSER_H

/* Parse the given tokens, and return the generated list.
 * Also, store the first unused token (or NULL) in the passed pointer.
 * This is used by looping with token list until it is set to null, as such:
 *
 *      TokenList* tokens = LexFile(my_file);
 *      while(tokens){
 *          VyObject parsed = Parse(tokens, &tokens);
 *          Eval(parsed); // Or whatever
 *      }
 */
VyObj Parse(TokenList* tokens, TokenList** used);

#endif /* PARSER_H */
