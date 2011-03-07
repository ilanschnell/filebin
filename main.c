/*
   Author: Ilan Schnell, 2008
   License: BSD
*/

#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* command line options */
int table = 0;
int names = 0;
int help = 0;

/* byte counts and total count */
long long bin[256], n = 0;


void usage(char *progname)
{
    printf("Usage: %s [-nthv] [file] ...\n", progname);
    printf("\n\
Calculates the frequency of bytes of file(s) or stdin.\n\
Options:\n\
  -n   show names when displaying table (implies -t)\n\
  -t   show table\n\
  -h   display this help and exit\n\
  -v   display version number and exit\n");
    exit(2);
}

/* changes the global variables: table, names, help */
void process_options(int argc, char *argv[])
{
    int op;

    while ((op = getopt(argc, argv, "nthv")) != -1)
        switch(op) {
        case 'n':
            names = table = 1;
            break;
        case 't':
            table = 1;
            break;
        case 'v':
            printf("filebin 1.0.0\n");
            exit(0);
        default:
            help = 1;
        }
}

void process_fp(FILE *fp)
{
    int c;

    while ((c = fgetc(fp)) != EOF) {
        bin[c]++;
        n++;
    }
}

void process_file(char *fname)
{
    FILE *fp;

    fp = fopen(fname, "rb");
    if (fp == NULL) {
        printf("Error: Can't open `%s' for reading.\n", fname);
        exit(1);
    }
    process_fp(fp);
    fclose(fp);
}

/* Returns the name of character c as a string,
   e.g. "A", "B", "<Space>", "<LF>"
   When the character has no specific name, "." is returned.
*/
char *name(int c)
{
    static char res[8];

    strcpy(res, ".");  /* default */

    if (c >= 33 && c <= 126) {
        res[0] = (char)c;
        res[1] = '\0';
    }
#define M(_c, _name)  else if (c==_c) strcpy(res, "<" _name ">")
    M(0, "Null");
    M(9, "Tab");
    M(10, "LF");
    M(13, "CR");
    M(32, "Space");
    M(127, "Del");
#undef M
    return res;
}

void showTable()
{
    int c;

    if (names)
        printf("Char               Count\n"
               "------------------------\n");

    for (c = 0; c < 256; c++)
        if (bin[c]) {
            if(names)
                printf("%4d %8s %10lld\n", c, name(c), bin[c]);
            else
                printf("%4d %10lld\n", c, bin[c]);
        }
}

void showSum()
{
    long long a, b1, b2;
    int c;

    a = 0;
    for (c = 32; c <= 126; c++)
        a += bin[c];
    printf("ASCII ( 32..126 )........  :%12lld\n", a);
    printf("   0 (Null)............... :%12lld\n", bin[0]);
    printf("   9 (Tab)................ :%12lld\n", bin[9]);
    printf("  10 (LF)................. :%12lld\n", bin[10]);
    printf("  13 (CR)................. :%12lld\n", bin[13]);
    printf(" 127 (Del) ............... :%12lld\n", bin[127]);

    b1 = bin[11]+bin[12];
    for(c = 1; c <=  8; c++) b1 += bin[c];
    for(c =14; c <= 31; c++) b1 += bin[c];
    printf("Others (1..8,11,12,14..31) :%12lld\n", b1);

    b2 = 0;
    for(c = 128; c <= 255; c++) b2 += bin[c];
    printf("Others (128..255) ........ :%12lld\n", b2);

    printf("                             -----------\n");
    printf("Total .................... :%12lld\n", n);

    assert(n == a + bin[0] + bin[9] + bin[10] + bin[13] + bin[127] + b1 + b2);
}

int main(int argc, char *argv[])
{
    int c;

    process_options(argc, argv);

    /* Initialize array */
    for (c = 0; c < 256; c++)
        bin[c] = 0;

    if (help)
        usage(argv[0]);
    else if (argc == optind)
        process_fp(stdin);
    else
        while (optind < argc)
            process_file(argv[optind++]);

    if (table)
        showTable();
    else
        showSum();

    exit(0);
}
