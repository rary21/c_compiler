#ifndef H_CC_PARS
#define H_CC_PARS
#include "cc_tkn.h"

typedef struct NODE{
    struct NODE **list;
    TOKEN *valtkn;
    int  list_count;
    int kind;
} NODE;

int m_cnt; // malloc count

NODE *buildAST();
void freeAST(NODE *);
void freeNodeRecursive(NODE *);
void freeNode(NODE *);
void freeTkn(TOKEN *);

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
void nodeAppend(NODE *node, NODE* next);

void *errorMsgAndFree(NODE *node, const char *kind);
#endif