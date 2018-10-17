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
    printf("list->");
    if (node->list)
        printf("some");
    else
        printf("NULL");

    printf("  listcnt->");
    printf("%d", node->list_count);

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

    //printf("   pointer %p\n", node);
}

void printNodeRecursive(NODE *top)
{
    NODE **list_top = top->list;
    //printf("%p %p\n", top->list, top);
    //printf("%d\n", top->kind);
    while (list_top && *list_top) {
        printNodeRecursive(*list_top);
        list_top++;
    }
    printNode(top);
}

void _printRPN(NODE *top)
{
    NODE **list_top = top->list;
    while (list_top && *list_top) {
        _printRPN(*list_top);
        list_top++;
    }
    if (top->valtkn)
        printf("%s ", top->valtkn->text);
    else if (top->kind != INVALID)
        printf("%s ", kindtext[top->kind]);
}

void printRPN(NODE *top)
{
    _printRPN(top);
    printf("\n");
}