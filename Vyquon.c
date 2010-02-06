#include "Vyquon.h"


int main(){
    FILE* file = fopen("test/test.vy", "r");
    TokenList* tokens = LexFile(file);

    TokenList* last = tokens;
    while(last != NULL){
        VyObj parsed = Parse(last, &last);
        PrintObj(stdout, parsed);
        printf("\nevaluates to\n");
        VyObj val = Eval(parsed);
        PrintObj(stdout, val);
        printf("\n\n");
    }

    FreeTokens(tokens);
    DEBUG_PRINT("Freed memory\n");
    /*
    SyntaxTree* tree = ParseTokenList(tokens);
    PrintSyntaxTree(stdout);
    */

    return 0;
}
