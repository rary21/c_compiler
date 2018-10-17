#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "cc_tkn.h"
#include "cc_print.h"

const char kindtext[NKIND + 1][10] = 
    {"digit", "ident", "plus", "minus", "mult", "divi", ">", ">=", "<", "<=", "==",
     "lpare", "rpare", "assign", "begin", "end", "semi", "dot", "compound", "noop",
     ""};
char ctype[256 + 1];
const char *opsText = "+-*/()";
const char *ctrlsText = ":=;";
TOKEN tkn, looktkn;
TOKEN tkns[NUM_TKN];
int numtkn;
int eof_found = 0;
FILE *fp;

void nextTkn()
{
    _nextTkn(0, 0);
}

void lookTkn(int nforward)
{
    _nextTkn(1, nforward);
}

void _nextTkn(int mode, int nforward)
{
    int i = 0;
    char c, tmptxt[TKN_SIZE + 1];
    TOKEN *l_tkn;

    if (mode)
        l_tkn = &looktkn;
    else
        l_tkn = &tkn;
    l_tkn->kind = INVALID;
    l_tkn->val  = 0;
    l_tkn->text[0] = '\0';
    
    // skip whitespace
    skipSpace();

    if (eof_found) {
        return;
    }

    c = getc(fp);
    if (isdigit(c)) {
        ungetc(c, fp);
        l_tkn->kind = digit;
        getInt(l_tkn);
    } else if (isalpha(c)) {
        ungetc(c, fp);
        getIdent(l_tkn);
    } else {
        l_tkn->kind = ctype[c];
        l_tkn->text[0] = c;
        l_tkn->text[1] = '\0';
        switch (c) {
        case ':':
            if (lookChar(1) != '=')
                break;
            l_tkn->kind = assign;
            strncpy(l_tkn->text, kindtext[l_tkn->kind], sizeof(kindtext[l_tkn->kind]));
            getc(fp);
            break;
        case '>':
            l_tkn->kind = gr;
            if (lookChar(1) != '=')
                break;
            l_tkn->kind = gre;
            strncpy(l_tkn->text, kindtext[l_tkn->kind], sizeof(kindtext[l_tkn->kind]));
            getc(fp);
            break;
        case '<':
            l_tkn->kind = le;
            if (lookChar(1) != '=')
                break;
            l_tkn->kind = lee;
            strncpy(l_tkn->text, kindtext[l_tkn->kind], sizeof(kindtext[l_tkn->kind]));
            getc(fp);
            break;
        case '=':
            if (lookChar(1) != '=')
                break;
            l_tkn->kind = equal;
            strncpy(l_tkn->text, kindtext[l_tkn->kind], sizeof(kindtext[l_tkn->kind]));
            getc(fp);
            break;
        default:
            break;
        }
    }
    if (mode) {
        strcpy(tmptxt, looktkn.text);
        if (nforward > 1)
            lookTkn(nforward - 1);
        ungetstr(tmptxt);
    }
#ifdef DEBUG
    if (!mode)
        printTkn(l_tkn);
#endif
}

char lookChar(int nforward)
{
    char c, _c;

    _c = getc(fp);
    if (nforward > 1)
        c = lookChar(nforward - 1);
    ungetc(_c, fp);
    if (nforward == 1)
        return _c;
    return c;
}

int isoperator(char c)
{
    int i = 0;
    while (opsText[i] && c != opsText[i]) i++;
    if (opsText[i])
        return 1;
    return 0;
}

int iscontrol(char c)
{
    int i = 0;
    while (ctrlsText[i] && c != ctrlsText[i]) i++;
    if (ctrlsText[i])
        return 1;
    return 0;
}

void getInt(TOKEN *_tkn)
{
    int i = 0, num = 0;
    char c;

    clearText(_tkn);
    while ((i < 10) && (c = getc(fp))) {
        if (c == EOF)
            break;
        if (isdigit(c)) {
            num = 10 * num + (c - '0');
            _tkn->text[i++] = c;
        } else {
            break;
        }
    }
    _tkn->val = num;

    ungetc(c, fp);
}

void getIdent(TOKEN *l_tkn)
{
    int i = 0;
    char c;

    c = getc(fp);
    if (!isalpha(c))
        return ;

    clearText(l_tkn);
    l_tkn->kind = ident;
    l_tkn->text[i++] = c;
    while ((c = getc(fp)) && i < TKN_SIZE && (isalpha(c) || isdigit(c)))
        l_tkn->text[i++] = c;
    l_tkn->text[i] = '\0';

    if (!strncmp(l_tkn->text, "BEGIN", sizeof("BEGIN")))
        l_tkn->kind = begin;
    if (!strncmp(l_tkn->text, "END", sizeof("END")))
        l_tkn->kind = end;
        

    ungetc(c, fp);
}

void openfile(const char *fname)
{
    if (!fname)
        fp = stdin;
    else if (!(fp = fopen(fname, "r")))
        perror("fopen");
}

void ungetstr(char *str)
{
    int i = 0;

    if (!str[0])
        return ;
    while (str[i]) i++;
    while (ungetc(str[--i], fp), i > 0);
}

void skipSpace()
{
    char c;
    while (isspace(c = getc(fp)));
    if (c == EOF) {
        eof_found = 1;
    }
    ungetc(c, fp);
}

void initCapture()
{
    char i;
    for (i = 0; i < sizeof(ctype); i++)
        ctype[i] = INVALID;

    for (i = '0'; i <= '9'; i++)
        ctype[i] = digit;
    for (i = 'a'; i <= 'z'; i++)
        ctype[i] = ident;
    for (i = 'A'; i <= 'Z'; i++)
        ctype[i] = ident;
    ctype['+'] = plus;  ctype['-'] = minus;
    ctype['*'] = mult;  ctype['/'] = divi;
    ctype['('] = lpare; ctype[')'] = rpare;
    ctype[';'] = semi;  ctype['.'] = dot;
}

int isvalidtkn(TOKEN _tkn)
{
    return _tkn.kind != INVALID && _tkn.kind != semi && _tkn.kind != end;
}

TOKEN *copyCurTkn()
{
    TOKEN *tmp;
    tmp = (TOKEN *)malloc(sizeof(TOKEN));
    memcpy(tmp, &tkn, sizeof(TOKEN));
    return tmp;
}

void clearText(TOKEN *l_tkn)
{
    int i;
    for (i = 0; i < TKN_SIZE; i++)
        l_tkn->text[i] = '\0';
}