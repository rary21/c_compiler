#ifndef H_CC_TKN
#define H_CC_TKN

#define NUM_TKN 100
#define TKN_SIZE 31

enum {digit, letter, plus, minus, mult, divi, 
      lpare, rpare, line_end, INVALID, NKIND};

typedef struct TOKEN {
    int kind;
    int val;
    char text[TKN_SIZE + 1];
} TOKEN;

void initCapture();
void openfile(const char *fname);
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

#endif