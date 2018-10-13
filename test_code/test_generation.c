#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_N 20
#define MAX_OPS 10
#define NUM_TEST 10
#define NUM_OP 4
#define NUM_PARE 10
#define INT_MAX 20000000
FILE *fp;

int seePare();
void pushPare(int);
void popPare();
void popAllPare();
void subPare();
void clearPare();

char ops[NUM_OP + 1] = "+-*/";
int pares[NUM_PARE] = {INT_MAX};
int ipare = 0;

int main(int argc, char **argv)
{
    int i, j, idx, _idx, n, nops, expc, nextpare = -1;
    int ispare, isendpare;

    if (argc < 2)
        fp = stdout;
    else
        fp = fopen(argv[1], "w");
    
    if (!fp)
        perror("fopen");

    srand((unsigned) time(NULL));
    for (i = 0; i < NUM_TEST; i++) {
        fprintf(fp, "%d ", rand() % MAX_N);
        expc = 0;
        nops = 1 + rand() % MAX_OPS; 
        ispare = 0;
        clearPare();
        for (j = 0; j < nops; j++) {
            idx = rand() % NUM_OP;
            // pare for 20% chance
            ispare = (rand() % 5) == 0 ? 1 : 0;
            if (ispare) {
                int tmp;
                tmp = 1 + rand() % 3;
                //fprintf(stderr, "j: %d pushPare(%d)\n", j, tmp);
                pushPare(tmp);
            }

            if (ops[idx] == '/') {
                n = 1 + rand() % 4; 
                if (n == 0)
                    n = 1;
                n *= 2;
            } else {
                n = 1 + rand() % MAX_N;
            }
            if (seePare() <= 0) {
                //fprintf(stderr, "j: %d seePare(): %d\n", j, seePare());
                fprintf(fp, ") ");
                popPare();
            } 
            subPare();

            fprintf(fp, "%c ", ops[idx]);
            if (ispare) {
                ispare = 0;
                fprintf(fp, "( ");
            }
            fprintf(fp, "%d ", n);

        }
        popAllPare();
        fprintf(fp, "; \n");
    }
}

void popPare()
{
    pares[--ipare] = -1;
}

void popAllPare()
{
    int i;

    for (i = 0; i < ipare; i++)
        fprintf(fp, ") ");
}

void pushPare(int i)
{
    pares[ipare++] = i;
}

int seePare()
{
    if (ipare <= 0)
        return INT_MAX;
    return pares[ipare - 1];
}

void subPare()
{
    int i;
    for (i = 0; i < NUM_PARE; i++)
        pares[i] -= 1;
}

void clearPare()
{
    int i;
    ipare = 0;
    for (i = 0; i < NUM_PARE; i++)
        pares[i] = INT_MAX;
}