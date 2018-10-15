#include <stdio.h>
#include "cc_tkn.h"
#include "cc_pars.h"

extern const char kindtext[NKIND + 1][10];

void printTkn(TOKEN *l_tkn)
{
    printf("token:  kind: \"%-10s\"", kindtext[l_tkn->kind]);
    printf(" text: %-10s", l_tkn->text);
    printf(" val : %d\n", l_tkn->val);
}

void printNode(NODE *node)
{
    printf("left->");
    if (node->left)
        printf("some");
    else
        printf("NULL");

    printf("  right->");
    if (node->right)
        printf("some");
    else
        printf("NULL");

    printf("  val->");
    if (node->valtkn)
        printf("%d", node->valtkn->val);
    else
        printf("NULL");

    printf("  op->");
    if (node->op)
        printf("%s\n", node->op->text);
    else
        printf("NULL\n");
}

void printNodeRecursive(NODE *top)
{
    if (top->left)
        printNodeRecursive(top->left);
    if (top->right)
        printNodeRecursive(top->right);
    printNode(top);
}

void _printRPN(NODE *top)
{
    if (top->left)
        _printRPN(top->left);
    if (top->right)
        _printRPN(top->right);
    if (top->valtkn)
        printf("%d ", top->valtkn->val);
    if (top->op)
        printf("%s ", top->op->text);
}

void printRPN(NODE *top)
{
    _printRPN(top);
    printf("\n");
}