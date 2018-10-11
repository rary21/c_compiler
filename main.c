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

enum {digit, letter, operand, control, INVALID, NKIND};
const char kindtext[NKIND + 1][10] = {"digit", "letter", "operand", "control", ""};
const char *opsText = "+-*/";
const char *ctrlsText = "=;";
TOKEN INVALID_TKN = {INVALID, 0, ""};
TOKEN tkn, looktkn;
TOKEN tkns[NUM_TKN];
int numtkn;
int eof_found = 0;

void openfile(const char *fname);
void execute();
void expression();
void term();
void factor();
void nextTkn();
void lookTkn();
void printTkn(TOKEN);
int isoperand(char c);
int iscontrol(char c);
int getInt();
void getIdent();

void skipSpace();

FILE *fp;

int main (int argc, const char* argv[])
{
    openfile(argv[1]);

    execute();

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
        //printf("%d\n", tkn.kind);
        //if (tkn.kind != letter) {
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
void expression()
{
    nextTkn();
    
}
void term()
{

}
void factor()
{

}

void nextTkn()
{
    int i = 0;
    char c, sc;
    tkn.kind = INVALID;
    tkn.val  = 0;
    tkn.text[0] = '\0';
    
    if (eof_found) {
        printf("----- end of file -----\n");
        return;
    }

    // skip whitespace
    skipSpace();

    c = getc(fp);
    if (isdigit(c)) {
        ungetc(c, fp);
        tkn.kind = digit;
        tkn.val = getInt();
    } else if (isalpha(c)) {
        ungetc(c, fp);
        tkn.kind = letter;
        getIdent(&tkn);
    } else if (isoperand(c)) {
        tkn.kind = operand;
        tkn.text[0] = c;
        tkn.text[1] = '\0';
    } else if (iscontrol(c)) {
        tkn.kind = control;
        tkn.text[0] = c;
        tkn.text[1] = '\0';
    } else if (c == EOF) {
        eof_found = 1;
    }
}

void lookTkn()
{
    int i = 0;
    char c, sc;
    tkn.kind = INVALID;
    tkn.val  = 0;
    tkn.text[0] = '\0';

    if (eof_found) {
        return;
    }

    // skip whitespace
    skipSpace();

    c = getc(fp);
    if (isdigit(c)) {
        ungetc(c, fp);
        tkn.kind = digit;
        tkn.val = getInt();
    } else if (isalpha(c)) {
        ungetc(c, fp);
        tkn.kind = letter;
        getIdent(&tkn);
    } else if (isoperand(c)) {
        tkn.kind = operand;
        tkn.text[0] = c;
        tkn.text[1] = '\0';
    } else if (iscontrol(c)) {
        tkn.kind = control;
        tkn.text[0] = c;
        tkn.text[1] = '\0';
    } else if (c == EOF) {
        eof_found = 1;
    }
    
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

int getInt()
{
    int i = 0, num = 0;
    char c;
    while ((i < 11) && (c = getc(fp))) {
        if (c == EOF)
            break;
        if (isdigit(c))
            num = 10 * num + (c - '0');
        else
            break;
    }
    ungetc(c, fp);
    return num;
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

void skipSpace()
{
    char c;
    while (isspace(c = getc(fp)));
    ungetc(c, fp);
}