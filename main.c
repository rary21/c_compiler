#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define NUM_TKN 100
#define TKN_SIZE 31

typedef struct TOKEN {
    int kind;
    int val;
    char text[TKN_SIZE + 1];
} TOKEN;

enum {digit, letter, plus, minus, mult, divi, 
      control, line_end, INVALID, NKIND};
char ctype[256 + 1];
const char kindtext[NKIND + 1][10] = 
    {"digit", "letter", "plus", "minus", "mult", "divi",
     "control", "line_end", ""};
const char *opsText = "+-*/";
const char *ctrlsText = "=;";
TOKEN INVALID_TKN = {INVALID, 0, ""};
TOKEN tkn, looktkn;
TOKEN tkns[NUM_TKN];
int numtkn;
int eof_found = 0;

void initCapture();
void openfile(const char *fname);
void execute();
int expression();
int term();
int factor();
void nextTkn();
void lookTkn(int);
void printTkn(TOKEN);
int isoperand(char c);
int iscontrol(char c);
int isvalidtkn(TOKEN);
void getInt(TOKEN *);
void getIdent(TOKEN *);
void ungetstr(char *);

void skipSpace();

FILE *fp;

int main (int argc, const char* argv[])
{
    int result;

    openfile(argv[1]);
    initCapture();

    //while (nextTkn(), tkn.kind != INVALID) {
    //    printTkn(tkn);
    //}
    //execute();
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
        //printf("%d\n", tkn.kind); //if (tkn.kind != letter) {
        //    printf("letter must be first\n");
        //    break;
        //}
        //nextTkn();
        //if (tkn.text[0] != '=') {
        //    printf("equal must be second\n");
        //    break;
        //}
        //expression();
    }
}
int expression()
{
    int result, tmp = 0;

    result = term();
    while (nextTkn(), isvalidtkn(tkn)) {
        switch (tkn.kind) {
        case plus:
            result += term();
            break;
        case minus:
            result -= term();
            break;
        default :
            break;
        }
    }

    return result;
}
int term()
{
    int result = 0;

    result = factor();
    if (tkn.kind != digit)
        printf("term() must start with digits\n");

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
    printf("term returns %d\n", result);
#endif
    return result;
}
int factor()
{
    nextTkn();
    if (tkn.kind == digit)
        return tkn.val;
    else
        printf("factor() error\n");
    return 0;
}

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
        printf("----- end of file -----\n");
        return;
    }

    c = getc(fp);
    if (isdigit(c)) {
        ungetc(c, fp);
        tkn.kind = digit;
        getInt(&tkn);
    } else if (isalpha(c)) {
        ungetc(c, fp);
        tkn.kind = letter;
        getIdent(&tkn);
    } else if (isoperand(c)) {
        tkn.kind = ctype[c];
        tkn.text[0] = c;
        tkn.text[1] = '\0';
    } else if (iscontrol(c)) {
        tkn.kind = ctype[c];
        tkn.text[0] = c;
        tkn.text[1] = '\0';
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
        looktkn.kind = letter;
        getIdent(&looktkn);
    } else if (isoperand(c)) {
        looktkn.kind = ctype[c];
        looktkn.text[0] = c;
        looktkn.text[1] = '\0';
    } else if (iscontrol(c)) {
        looktkn.kind = ctype[c];
        looktkn.text[0] = c;
        looktkn.text[1] = '\0';
    }

    strcpy(tmptxt, looktkn.text);

//#ifdef DEBUG
//    printTkn(looktkn);
//#endif
    if (nfoward > 1)
        lookTkn(nfoward - 1);

    ungetstr(tmptxt);
}

void printTkn(TOKEN tkn)
{
    printf("token:  kind: \"%-10s\"", kindtext[tkn.kind]);
    printf(" text: %-10s", tkn.text);
    printf(" val : %d\n", tkn.val);
}

int isoperand(char c)
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
    tkn->text[i++] = c;
    while ((c = getc(fp)) && i < TKN_SIZE && (isalpha(c) || isdigit(c)))
        tkn->text[i++] = c;
    tkn->text[i] = '\0';

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
        ctype[i] = letter;
    for (i = 'A'; i <= 'Z'; i++)
        ctype[i] = letter;
    ctype['+'] = plus; ctype['-'] = minus;
    ctype['*'] = mult; ctype['/'] = divi;
    ctype[';'] = line_end;
}

int isvalidtkn(TOKEN _tkn)
{
    return _tkn.kind != INVALID && _tkn.kind != line_end;
}