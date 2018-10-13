#include <stdio.h>
#include "cc_tkn.h"
#include "cc_pars.h"
#include "cc_eval.h"

#define STACK_SIZE 100

TOKEN *stack[STACK_SIZE];
int istack = 0;

int evalAST(NODE *top)
{
    evalRPN(top);
    return popTkn()->val;
}

void evalRPN(NODE *top)
{
    int result;
    if (top->left)
        evalRPN(top->left);
    if (top->right)
        evalRPN(top->right);
    if (top->valtkn)
        pushTkn(top->valtkn);

    // evaluate operators
    if (top->op) {
        int result;
        TOKEN *t1, *t2;
        t2 = popTkn();
        t1 = popTkn();
        result = evalOP(t1->val, t2->val, top->op->kind);
        //printf("v1:%d v2:%d op:%s result: %d\n",
        //       t1->val, t2->val, top->op->text, result);
        t1->val = result;
        pushTkn(t1);
    }
}

int evalOP(int val1, int val2, int op)
{
    switch (op) {
    case plus:
        return val1 + val2;
        break;
    case minus:
        return val1 - val2;
        break;
    case mult:
        return val1 * val2;
        break;
    case divi:
        if (val2 == 0) {
            perror("error: zero division\n");
            return 0;
        }
        return val1 / val2;
        break;
    default:
        break;
    }
    perror("evalOP");
    return 0;
}

void pushTkn(TOKEN *_tkn) {
    stack[istack++] = _tkn;
}

TOKEN *popTkn()
{
    if (istack == 0)
        return (TOKEN *)NULL;
    return stack[--istack];
}