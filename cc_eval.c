#include <stdio.h>
#include <assert.h>
#include "cc_tkn.h"
#include "cc_pars.h"
#include "cc_eval.h"

#define STACK_SIZE 100
#define NOT_FOUND 0
#define FOUND 1

extern const char kindtext[NKIND + 1][10];
TOKEN *stack[STACK_SIZE];
int g_table[25] = {0};
Stack varStack = {{0}, -1};
int istack = 0;


int evalAST(NODE *top)
{
    int result;
    evalRPN(top);
    //result = popTkn()->val;
    assert(istack == 0);
    return result;
}

void evalRPN(NODE *top)
{
    NODE **list_top = top->list;
    while (list_top && *list_top) {
        evalRPN(*list_top);
        list_top++;
    }

    if (top->valtkn)
        pushTkn(top->valtkn);
    // evaluate operators
    if (top->kind != INVALID)
        evalOP(top);
}

void pushTkn(TOKEN *_tkn) 
{
    stack[istack++] = _tkn;
}

TOKEN *popTkn()
{
    if (istack == 0)
        return (TOKEN *)NULL;
    return stack[--istack];
}

void evalOP(NODE *node)
{
    int opnum = opnumber(node);
    if (opnum == unary) {
        int result;
        TOKEN *t;
        t = popTkn();
        result = evalOP1(t, node->kind);
        //printf("v:%s op:%s result: %d\n",
        //       t->text, kindtext[node->kind], result);
        t->val = result;
        t->kind = digit;
        pushTkn(t);
    } else if (opnum == binary) {
        int result;
        TOKEN *t1, *t2;
        t2 = popTkn();
        t1 = popTkn();
        result = evalOP2(t1, t2, node->kind);
        //printf("v1:%d v2:%d op:%s result: %d\n",
        //       t1->val, t2->val, kindtext[node->kind], result);
        if (node->kind != assign) {
            t1->val = result;
            t1->kind = digit;
            pushTkn(t1);
        } else {
            addTable(t1->val);
        }
    } else {
        //perror("op must take at least one operand\n");
    }
}

int evalOP1(TOKEN *l_tkn, int op)
{
    int *val;
    val = getReference(l_tkn, 0);
    switch (op) {
    case plus:
        return *val;
        break;
    case minus:
        return -*val;
        break;
    default:
        break;
    }
    perror("evalOP1");
    return 0;
}

int evalOP2(TOKEN *val1, TOKEN *val2, int op)
{
    int *v1, *v2;
    v1 = getReference(val1, op == assign);
    v2 = getReference(val2, 0);
    //if (!v1 || !v2)
    //    return 0;

    switch (op) {
    case plus:
        return *v1 + *v2;
        break;
    case minus:
        return *v1 - *v2;
        break;
    case mult:
        return *v1 * *v2;
        break;
    case divi:
        if (*v2 == 0) {
            perror("error: zero division\n");
            return 0;
        }
        return *v1 / *v2;
        break;
    case assign:
        *v1 = *v2;
        return *v1;
    default:
        break;
    }
    perror("evalOP2");
    return 0;
}

int opnumber(NODE *node)
{
    int num;
    if (node->list_count == 2)
        return binary;
    if (node->list_count == 1)
        return unary;
    return none;
}

void displayVars()
{
    int i;
    printf("--- ");
    for (i = 0; i < 25; i++)
        if (findStack(&varStack, i))
            printf("[%c]=%d ", i + 'a', g_table[i]);
    printf(" ---\n");
}

void pushStack(Stack *_stack, int val)
{
    if (_stack->cnt < STACK_SIZE - 1)
        _stack->s[++_stack->cnt] = val;
    else
        perror("pushstack");
}

int popStack(Stack *_stack)
{
    if (_stack->cnt >= 0)
        return _stack->s[_stack->cnt--];
    else
        perror("popstack");
    return 0;
}

void addTable(int id)
{
    if (findStack(&varStack, id) == NOT_FOUND) {
        // add to table
        pushStack(&varStack, id);
        printf("[%c] is added to table\n", id + 'a');
    }
}

int findStack(Stack *_stack, int id)
{
    int i;

    for (i = 0; i <= _stack->cnt; i++)
        if (_stack->s[i] == id)
            return FOUND;
    return NOT_FOUND;
}

int isinTable(TOKEN *l_tkn)
{
    return findStack(&varStack, l_tkn->val) == FOUND;
}

int *getAddress(TOKEN *l_tkn)
{
    return &g_table[l_tkn->val];
}

// mode: 0 - 
int *getReference(TOKEN *l_tkn, int mode)
{
    if (isident(l_tkn)) {
        if (mode || isinTable(l_tkn)) {
            return &g_table[l_tkn->val];
        } else {
            printf("error: undefined variable [%c]\n", l_tkn->val + 'a');
            return NULL;
        }
    } else {
        return &l_tkn->val;
    }
}