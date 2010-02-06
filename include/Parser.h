#ifndef PARSER_H
#define PARSER_H

/* Parse the given tokens, and return the generated list.
   Also, store the first unused token (or NULL) in the passed pointer.
*/
VyObj Parse(TokenList* tokens, TokenList** used);

#endif /* PARSER_H */
