#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "cc_tkn.h"

#define NUM_TKN 100
#define TKN_SIZE 31

void execute();
int expression();
int term();
int factor();
extern TOKEN tkn, looktkn;

int main (int argc, const char* argv[])
{
    int result;

    openfile(argv[1]);
    initCapture();

    while (1) {
        lookTkn(1);
        if (looktkn.kind == INVALID)
            break;
        result = expression();
        printf("%d\n", result);
    }
    return 0;
}

void execute()
{
    int i;

    while (1) {
        nextTkn();
        if (tkn.kind == INVALID)
            break;
        printTkn(tkn);
    }
}

int expression()
{
    int result, tmp = 0;

    result = term();
    while (lookTkn(1), isvalidtkn(looktkn)) {
        switch (looktkn.kind) {
        case plus:
            nextTkn();
            result += term();
            break;
        case minus:
            nextTkn();
            result -= term();
            break;
        case rpare:
            return result;
            break;
        default :
            break;
        }
    }
    nextTkn();
#ifdef DEBUG
    printf("exps  returns %d\n", result);
#endif

    return result;
}
int term()
{
    int result = 0;

    result = factor();
    while (lookTkn(1), isvalidtkn(looktkn)) {
        if (looktkn.kind != mult && looktkn.kind != divi)
            goto BREAK;
        nextTkn();
        switch (tkn.kind) {
        case mult:
            result *= factor();
            break;
        case divi:
            result /= factor();
            break;
        default :
            goto BREAK;
        }
    }

BREAK:
#ifdef DEBUG
    printf("term  returns %d\n", result);
#endif
    return result;
}

int factor()
{
    int result = 0;
    nextTkn();
    if (tkn.kind == digit)
        return tkn.val;
    else if (tkn.kind == lpare)
        result = expression(); 
    else
        printf("factor() error\n");
    
    nextTkn();
    if (tkn.kind != rpare)
        printf("factor() pare error\n");

#ifdef DEBUG
    printf("factor returns %d\n", result);
#endif
    return result;
}