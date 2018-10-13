#ifndef H_CC_EVAL
#define H_CC_EVAL
#include "cc_tkn.h"
#include "cc_pars.h"

int evalAST(NODE *);
void evalRPN(NODE *);
int evalOP(int val1, int val2, int op);
void pushTkn(TOKEN *);
TOKEN *popTkn();
#endif