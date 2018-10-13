#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "cc_tkn.h"
#include "cc_pars.h"

extern TOKEN tkn, looktkn;
extern int eof_found;

NODE *buildAST()
{
    return expression();
}

void freeAST(NODE *top)
{
    freeNextNode(top);
}

void freeNextNode(NODE *cur)
{
    if (cur->left)
        freeNextNode(cur->left);
    if (cur->right)
        freeNextNode(cur->right);

    if (cur->valtkn)
        free(cur->valtkn);
    if (cur->op)
        free(cur->op);
    free(cur);
}

NODE *createNode()
{
    NODE *node = (NODE *)malloc(sizeof(NODE));
    node->left   = (NODE *)NULL;
    node->right  = (NODE *)NULL;
    node->op     = (TOKEN *)NULL;
    node->valtkn = (TOKEN *)NULL;
    return node;
}

NODE *expression()
{
    NODE *node, *pnode;

    node = createNode();
    node->left = term();
    if (!node->left)
        return NULL;
    while (lookTkn(1), isvalidtkn(looktkn)) {
        switch (looktkn.kind) {
        case plus:
        case minus:
            nextTkn();
            node->op = copyCurTkn();
            node->right = term();
            pnode = createNode();
            pnode->left = node;
            node = pnode;
            break;
        case rpare:
            return node;
            break;
        default :
            break;
        }
    }
    nextTkn();
#ifdef DEBUG
    printf("exps  returns %d\n", result);
#endif

    node = cutRedundancy(node);
    return node;
}
NODE *term()
{
    NODE *node, *pnode;

    node = createNode();

    node = createNode();
    node->left = factor();
    if (!node->left)
        return NULL;
    while (lookTkn(1), isvalidtkn(looktkn)) {
        if (looktkn.kind != mult && looktkn.kind != divi)
            goto BREAK;
        nextTkn();
        switch (tkn.kind) {
        case mult:
        case divi:
            node->op = copyCurTkn();
            node->right = factor();
            pnode = createNode();
            pnode->left = node;
            node = pnode;
            break;
        default :
            goto BREAK;
        }
    }

BREAK:
#ifdef DEBUG
    printf("term  returns %d\n", result);
#endif

    node = cutRedundancy(node);
    return node;
}

NODE *factor()
{
    NODE *node;

    node = createNode();
    nextTkn();
    if (eof_found)
        return NULL;
    if (tkn.kind == digit) {
        node->valtkn = copyCurTkn();
        return node;
    } else if (tkn.kind == lpare) {
        node->left = expression(); 
    } else {
        printf("factor() must digit or paren\n");
    }
    
    nextTkn();
    if (tkn.kind != rpare)
        printf("factor() paren must match\n");

#ifdef DEBUG
    printf("factor returns %d\n", result);
#endif
    return node;
}

NODE *cutRedundancy(NODE *node)
{
    NODE *ret;

    ret = node;
    assert(node->left  || node->right);

    if (node->left && !node->right) {
        ret = node->left;
        free(node);
    } else if (!node->left && node->right) {
        ret = node->right;
        free(node);
    }
    return ret;
}
