#ifndef H_CC_EVAL
#define H_CC_EVAL
#include "cc_tkn.h"
#include "cc_pars.h"

typedef struct Stack {
    int s[100];
    int cnt;
} Stack;

int evalAST(NODE *);
void evalRPN(NODE *);
void evalOP(NODE *);
int evalOP1(TOKEN *val, int op);
int evalOP2(TOKEN *val1, TOKEN *val2, int op);
void pushTkn(TOKEN *);
TOKEN *popTkn();
int opnumber(NODE *);

void displayVars();
void addTable(int id);

void pushStack(Stack *_stack, int val);
int popStack(Stack *_stack);
int findStack(Stack *_stack, int id);
#endif