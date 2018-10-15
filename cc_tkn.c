#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "cc_tkn.h"
#include "cc_print.h"

const char kindtext[NKIND + 1][10] = 
    {"digit", "ident", "plus", "minus", "mult", "divi", "gr", "gre", "le", "lee", "equal",
     "lpare", "rpare", "assign", "begin", "end", "semi", "dot", 
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
    int i = 0;
    char c, sc;
    tkn.kind = INVALID;
    tkn.val  = 0;
    tkn.text[0] = '\0';
    
    // skip whitespace
    skipSpace();

    if (eof_found) {
        return;
    }

    c = getc(fp);
    if (isdigit(c)) {
        ungetc(c, fp);
        tkn.kind = digit;
        getInt(&tkn);
    } else if (isalpha(c)) {
        ungetc(c, fp);
        getIdent(&tkn);
    } else {
        tkn.kind = ctype[c];
        tkn.text[0] = c;
        tkn.text[1] = '\0';
        switch (c) {
        case ':':
            if (lookChar(1) != '=')
                break;
            getc(fp);
            tkn.kind = assign;
            break;
        case '>':
            tkn.kind = gr;
            if (lookChar(1) != '=')
                break;
            tkn.kind = gre;
            getc(fp);
            break;
        case '<':
            tkn.kind = le;
            if (lookChar(1) != '=')
                break;
            tkn.kind = lee;
            getc(fp);
            break;
        case '=':
            if (lookChar(1) != '=')
                break;
            getc(fp);
            tkn.kind = equal;
            break;

        }
    }
#ifdef DEBUG
    printTkn(tkn);
#endif
}

void lookTkn(int nfoward)
{
    int i = 0;
    char c, sc, tmptxt[TKN_SIZE + 1];
    looktkn.kind = INVALID;
    looktkn.val  = 0;
    looktkn.text[0] = '\0';

    // skip whitespace
    skipSpace();

    if (eof_found) {
        return;
    }

    c = getc(fp);
    if (isdigit(c)) {
        ungetc(c, fp);
        looktkn.kind = digit;
        getInt(&looktkn);
    } else if (isalpha(c)) {
        ungetc(c, fp);
        getIdent(&looktkn);
    } else if (isoperator(c) || iscontrol(c)) {
        if (c == ':' && lookChar(1) == '=') {
            getc(fp);
            tkn.kind = assign;
        } else {
            looktkn.kind = ctype[c];
            looktkn.text[0] = c;
            looktkn.text[1] = '\0';
        }
    }
    strcpy(tmptxt, looktkn.text);

//#ifdef DEBUG
//    printTkn(looktkn);
//#endif
    if (nfoward > 1)
        lookTkn(nfoward - 1);

    ungetstr(tmptxt);
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

void getIdent(TOKEN *tkn)
{
    int i = 0;
    char c;

    c = getc(fp);
    if (!isalpha(c))
        return ;

    tkn->kind = ident;
    tkn->text[i++] = c;
    while ((c = getc(fp)) && i < TKN_SIZE && (isalpha(c) || isdigit(c)))
        tkn->text[i++] = c;
    tkn->text[i] = '\0';

    if (!strncmp(tkn->text, "BEGIN", sizeof("BEGIN")))
        tkn->kind = begin;
    if (!strncmp(tkn->text, "END", sizeof("END")))
        tkn->kind = end;
        

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
    return _tkn.kind != INVALID && _tkn.kind != semi;
}

TOKEN *copyCurTkn()
{
    TOKEN *tmp;
    tmp = (TOKEN *)malloc(sizeof(TOKEN));
    memcpy(tmp, &tkn, sizeof(TOKEN));
    return tmp;
}