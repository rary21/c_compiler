#ifndef H_CC_PARS
#define H_CC_PARS
#include "cc_tkn.h"

typedef struct NODE{
    struct NODE *left;
    struct NODE *right;
    struct NODE *children;
    TOKEN *valtkn;
    int kind;
} NODE;

NODE *buildAST();
void freeAST(NODE *);
void freeNextNode(NODE *);

NODE *program();
NODE *compound_statement();
NODE *statement_list();
NODE *statement();
NODE *assignment_statement();
NODE *empty();

NODE *expression();
NODE *term();
NODE *factor();
NODE *cutRedundancy(NODE *);

void *errorMsgAndFree(NODE *node, const char *kind);
#endif