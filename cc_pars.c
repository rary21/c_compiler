#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cc_tkn.h"
#include "cc_pars.h"
#include "cc_print.h"

extern TOKEN tkn, looktkn;
extern int eof_found;
extern const char kindtext[NKIND + 1][10];

NODE *buildAST()
{
    NODE *node;

    if (lookTkn(1), looktkn.kind == INVALID)
        return NULL;
    node = program();
    return node;
}

void freeAST(NODE *top)
{
    freeNodeRecursive(top);
    assert(m_cnt == 0);
}

void freeNodeRecursive(NODE *cur)
{
    int i;
    NODE **toplist = cur->list;
    while (toplist && *toplist) {
        freeNodeRecursive(*toplist);
        toplist++;
    }

    if (cur->list) {
        free(cur->list);
        m_cnt--;
    }
    if (cur->valtkn)
        freeTkn(cur->valtkn);
    freeNode(cur);
}

void freeNode(NODE *node)
{
    free(node);
    m_cnt--;
}

void freeTkn(TOKEN *l_tkn)
{
    free(l_tkn);
    m_cnt--;
}

NODE *createNode()
{
    NODE *node = (NODE *)malloc(sizeof(NODE));
    node->list  = (NODE **)NULL;
    node->valtkn = (TOKEN *)NULL;
    node->kind     = INVALID;
    node->list_count = 0;
    m_cnt++;
    return node;
}

NODE *program()
{
#ifdef DEBUG
    printf("[in program] \n");
#endif
    NODE *node = NULL;

    node = compound_statement();
    if (!node)
        return NULL;

    nextTkn();
    if (tkn.kind != dot)
        return errorMsgAndFree(node, "DOT : program");
    return node;
}

NODE *compound_statement()
{
#ifdef DEBUG
    printf("[in compound_statement] \n");
#endif
    NODE *node = createNode();

    nextTkn();
    if (tkn.kind != begin)
        return errorMsgAndFree(node, "BEGIN : compound_statement");
    nodeAppend(node, statement_list());

    nextTkn();
    if (tkn.kind != end)
        return errorMsgAndFree(node, "END : compound_statement");

#ifdef DEBUG
    printf("[end compound_statement] \n");
#endif
    return node;
}

NODE *statement_list()
{
#ifdef DEBUG
    printf("[in statement_list] \n");
#endif
    NODE *cnode;
    NODE *node = createNode();

    nodeAppend(node, statement());
    lookTkn(1);
    while (looktkn.kind == semi) {
        nextTkn();
#ifdef DEBUG
        printf("[new statement_list] \n");
#endif
        nodeAppend(node, statement());
        lookTkn(1);
    }

#ifdef DEBUG
    printf("[end statement_list]\n");
#endif
    return node;
}

NODE *statement()
{
#ifdef DEBUG
    printf("[in statement] \n");
#endif
    NODE *node;

    lookTkn(1);
    if (looktkn.kind == begin)
        node = compound_statement();
    else if (looktkn.kind == ident)
        node = assignment_statement();
    else
        node = empty();
        
    return node;
}

NODE *assignment_statement()
{
#ifdef DEBUG
    printf("[in assignment_statement] \n");
#endif
    NODE *node = createNode();
    NODE *cnode;
    
    nextTkn();
    if (tkn.kind != ident)
        return errorMsgAndFree(node, "IDENTIFIER : assignment_statement");
    cnode = createNode();
    cnode->kind = ident;
    cnode->valtkn = copyCurTkn();
    nodeAppend(node, cnode);

    nextTkn();
    if (tkn.kind != assign)
        return errorMsgAndFree(node, "ASSIGN : assignment_statement");

    node->kind = assign;
    cnode = expression();
    nodeAppend(node, cnode);

    return node;
}

NODE *empty()
{
#ifdef DEBUG
    printf("[in empty] \n");
#endif
    NODE *node = createNode();
    node->kind = noop;
    return node;
}

NODE *expression()
{
    NODE *node, *pnode;

    node = createNode();
    nodeAppend(node, term());
    if (!node->list) {
        freeNodeRecursive(node);
        return NULL;
    }
    lookTkn(1);
    while (lookTkn(1), isvalidtkn(looktkn)) {
        switch (looktkn.kind) {
        case plus:
        case minus:
            nextTkn();
            node->kind = tkn.kind;
            nodeAppend(node, term());
            pnode = createNode();
            nodeAppend(pnode, node);
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
    nodeAppend(node, factor());
    if (!node->list) {
        freeNodeRecursive(node);
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
            nodeAppend(node, factor());
            pnode = createNode();
            nodeAppend(pnode, node);
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
    case ident:
    case digit:
        node->valtkn = copyCurTkn();
        return node;
    case lpare:
        freeNode(node);
        node = expression(); 
        nextTkn();
        if (tkn.kind != rpare) {
            printf("factor() paren must match\n");
            freeNode(node);
            return NULL;
        }
        break;
    case plus:
    case minus:
        node->kind = tkn.kind;
        nodeAppend(node, expression());
        break;
    default:
        printf("factor() must digit or paren\n");
        freeNode(node);
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
    if (node->list_count == 1) {
        ret = *node->list;
        freeNode(node);
        free(node->list);
        m_cnt--;
    }
    return ret;
}

void *errorMsgAndFree(NODE *node, const char *kind)
{
    freeNode(node);
    fprintf(stderr, "error with token type : %s\n", kind);
    return NULL;
}

void nodeAppend(NODE *node, NODE* next)
{
    int len = node->list_count;

    if (!next) {
        perror("error: trying to append NULL pointer\n");
        return ;
    }
    if (len == 0)
        m_cnt++;
    // reallocate memory
    node->list = (NODE **)realloc(node->list, (len + 2) * sizeof(NODE *));

    *(node->list + node->list_count) = next;
    *(node->list + node->list_count + 1) = NULL;
    node->list_count++;
}