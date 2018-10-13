#ifndef H_CC_PARS
#define H_CC_PARS
#include "cc_tkn.h"

typedef struct NODE{
    struct NODE *left;
    struct NODE *right;
    TOKEN *op;
    TOKEN *valtkn;
} NODE;

NODE *buildAST();
void freeAST(NODE *);
void freeNextNode(NODE *);

NODE *execute();
NODE *expression();
NODE *term();
NODE *factor();
NODE *cutRedundancy(NODE *);

#endif