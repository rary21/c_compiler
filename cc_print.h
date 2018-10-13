#ifndef H_CC_PRINT
#define H_CC_PRINT

#include "cc_tkn.h"
#include "cc_pars.h"
void printTkn(TOKEN);
void printNode(NODE *);
void printNodeRecursive(NODE *);
void printRPN(NODE *);
void _printRPN(NODE *);

#endif