#ifndef H_CC_TKN
#define H_CC_TKN

#define NUM_TKN 100
#define TKN_SIZE 31

enum {none, unary, binary};
enum {digit, ident, plus, minus, mult, divi, gr, gre, le, lee, equal,
      lpare, rpare, assign, begin, end, semi, dot, INVALID, NKIND};

typedef struct TOKEN {
    int kind;
    int val;
    char text[TKN_SIZE + 1];
} TOKEN;

void initCapture();
void openfile(const char *fname);
void _nextTkn(int mode, int nfoward);
void nextTkn();
void lookTkn(int);
char lookChar(int);
int isoperator(char c);
int iscontrol(char c);
int isvalidtkn(TOKEN);
void getInt(TOKEN *);
void getIdent(TOKEN *);
void ungetstr(char *);
void skipSpace();
TOKEN *copyCurTkn();

#endif