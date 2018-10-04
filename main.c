#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define NUM_TKN 100
#define TKN_SIZE 31

typedef struct TOKEN {
    int kind;
    int val;
    char text[TKN_SIZE];
} TOKEN;

enum {digit, letter, control, INVALID, NKIND};
const char kindtext[NKIND + 1][10] = {"digit", "letter", "control", ""};
const char *ctrlsText = "+-*/=;";
TOKEN INVALID_TKN = {INVALID, 0, ""};

TOKEN nextTkn();
void printTkn(TOKEN);
int iscontrol(char c);

FILE *fp;

int main (int argc, const char* argv[])
{
    int itkn, i;
    TOKEN tkn, tkns[NUM_TKN];

    if (!(fp = fopen(argv[1], "r")))
        perror("fopen");

    itkn = 0;
    while ((tkn = nextTkn()).kind != INVALID)
        tkns[itkn++] = tkn;

    for (i = 0; i < itkn; i++) {
        printf("%d-th tokens \n", i+1);
        printTkn(tkns[i]);
    }
}

TOKEN nextTkn()
{
    int i = 0;
    char c, sc;
    TOKEN tkn = {INVALID, 0, ""};

    // skip whitespace
    while (isspace(c = getc(fp)));
    ungetc(c, fp);

    while ((i < TKN_SIZE) && (c = getc(fp))) {
        if (c == EOF)
            break;
        if (!isspace(c))
            tkn.text[i++] = c;
        else
            break;
    }

    sc = tkn.text[0];
    tkn.text[i] = '\0';
    if (isdigit(sc)) {
        tkn.kind = digit;
        tkn.val = atoi(tkn.text);
    } else if (isalpha(sc)) {
        tkn.kind = letter;
        tkn.val = strlen(&tkn.text[0]);
    } else if (iscontrol(sc)) {
        tkn.kind = control;
    }
    return tkn;
}

void printTkn(TOKEN tkn)
{
    printf("token:  kind: \"%-10s\"", kindtext[tkn.kind]);
    printf(" text: %-10s", tkn.text);
    printf(" val : %d\n", tkn.val);
}

int iscontrol(char c)
{
    int i = 0;
    while (ctrlsText[i] && c != ctrlsText[i]) i++;

    if (ctrlsText[i])
        return 1;
    return 0;
}