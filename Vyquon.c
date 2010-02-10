#include "Vyquon.h"

/* Main file */

/* Library loading is done somewhere else in external/ */
extern void LoadCoreLibrary();
void FinishRuntime();

int main(){
    /* Load library */
    LoadCoreLibrary();

    /* Open our file and lexify it */
    FILE* file = fopen("test/test.vy", "r");
    TokenList* tokens = LexFile(file);

    TokenList* last = tokens;
    while(last){
        VyObj parsed = Parse(last, &last);

        printf("|");
        PrintObj(stdout, parsed);
        printf("|\nevaluates to\n");

        VyObj val = Eval(parsed);

        PrintObj(stdout, val);
        printf("\n\n");
    }

    FreeTokens(tokens);

    /* Finalize */
    FinishRuntime();

    /* Exit */
    return 0;
}

/* Close all open resources and such */
void FinishRuntime(){
    DeleteInternedSymbols();
}
