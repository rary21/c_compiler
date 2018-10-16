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
    NODE *node;

    node = expression();
    nextTkn();
    return node;
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
    free(cur);
}

NODE *createNode()
{
    NODE *node = (NODE *)malloc(sizeof(NODE));
    node->left   = (NODE *)NULL;
    node->right  = (NODE *)NULL;
    node->valtkn = (TOKEN *)NULL;
    node->kind     = INVALID;
    return node;
}

NODE *program()
{
    NODE *node = createNode();

    node->left = compound_statement();
    if (tkn.kind != dot)
        return errorMsgAndFree(node, "DOT");
    return node;
}

NODE *compound_statement()
{
    NODE *node = createNode();

    nextTkn();
    if (tkn.kind != begin)
        return errorMsgAndFree(node, "BEGIN");
    node->left = statement_list();

    nextTkn();
    if (tkn.kind != end)
        return errorMsgAndFree(node, "END");

    return node;
}

NODE *statement_list()
{
    NODE *pnode;
    NODE *node = createNode();

    node->left = statement();

    nextTkn();
    while (tkn.kind == semi) {
        pnode = createNode();
        pnode->left = statement();
    }

    return node;
}

NODE *statement()
{
    NODE *node = createNode();

    return node;
}

NODE *assignment_statement()
{
    NODE *node = createNode();
    
    nextTkn();
    if (tkn.kind != ident)
        return errorMsgAndFree(node, "IDENTIFIER");
    node->left = createNode();
    node->left->valtkn = copyCurTkn();

    nextTkn();
    if (tkn.kind != equal)
        return errorMsgAndFree(node, "EQUAL");

    node->kind = tkn.kind;
    node->right = expression();

    return node;
}

NODE *expression()
{
    NODE *node, *pnode;

    node = createNode();
    node->left = term();
    if (!node->left) {
        free(node);
        return NULL;
    }
    while (lookTkn(1), isvalidtkn(looktkn)) {
        switch (looktkn.kind) {
        case plus:
        case minus:
            nextTkn();
            node->kind = tkn.kind;
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
#ifdef DEBUG
    //printf("exps  returns %d\n", result);
#endif

    node = cutRedundancy(node);
    return node;
}
NODE *term()
{
    NODE *node, *pnode;

    node = createNode();
    node->left = factor();
    if (!node->left) {
        free(node);
        return NULL;
    }
    while (lookTkn(1), isvalidtkn(looktkn)) {
        if (looktkn.kind != mult && looktkn.kind != divi)
            goto BREAK;
        nextTkn();
        switch (tkn.kind) {
        case mult:
        case divi:
            node->kind = tkn.kind;
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
    //printf("term  returns %d\n" );
#endif

    node = cutRedundancy(node);
    return node;
}

NODE *factor()
{
    NODE *node, *pnode;

    node = createNode();
    nextTkn();
    if (!isvalidtkn(tkn)) {
        return NULL;
    }

    switch (tkn.kind) {
    case digit:
        node->valtkn = copyCurTkn();
        return node;
    case lpare:
        free(node);
        node = expression(); 
        nextTkn();
        if (tkn.kind != rpare) {
            printf("factor() paren must match\n");
            free(node);
            return NULL;
        }
        break;
    case plus:
    case minus:
        node->kind = tkn.kind;
        node->left = expression();
        break;
    default:
        printf("factor() must digit or paren\n");
        free(node);
        return NULL;
    }
    
#ifdef DEBUG
    //printf("factor returns %d\n", result);
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

void *errorMsgAndFree(NODE *node, const char *kind)
{
    free(node);
    fprintf(stderr, "error with token type : %s\n", kind);
    return NULL;
}