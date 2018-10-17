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
    printf("children->");
    if (node->children)
        printf("some");
    else
        printf("NULL");

    printf("  left->");
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
        printf("%s", node->valtkn->text);
    else
        printf("NULL");

    printf("  op->");
    if (node->kind != INVALID)
        printf("%s\n", kindtext[node->kind]);
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
    if (top->children)
        printNodeRecursive(top->children);
}

void _printRPN(NODE *top)
{
    if (top->left)
        _printRPN(top->left);
    if (top->right)
        _printRPN(top->right);
    if (top->valtkn)
        printf("%s ", top->valtkn->text);
    if (top->kind != INVALID)
        printf("%s ", kindtext[top->kind]);
    if (top->children)
        _printRPN(top->children);
}

void printRPN(NODE *top)
{
    _printRPN(top);
    printf("\n");
}