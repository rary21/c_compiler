#include <stdio.h>
#include <assert.h>
#include "cc_tkn.h"
#include "cc_pars.h"
#include "cc_print.h"
#include "cc_eval.h"

extern TOKEN tkn, looktkn;
extern int m_cnt;

int main (int argc, const char* argv[]){
    int result;
    NODE *AST;
    
    openfile(argv[1]);
    initCapture();
    while (1) {
        AST = buildAST();
        if (!AST)
            break;
        
#ifdef DEBUG
        //printNode(AST);
        printNodeRecursive(AST);
        printRPN(AST);
#endif
        //printf("%d\n", evalAST(AST));
        evalAST(AST);
        displayVars();

        freeAST(AST);
    }
    return 0;
}