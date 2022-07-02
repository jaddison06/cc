#include <stdio.h>
#include <string.h>

#include "scanner.h"

static char* names[] = {
    "lbrace","rbrace","lsquare","rsquare",
    "lparen","rparen","comma","dot","star",
    "slash","minus","plus","greater-than",
    "less-than","equals","hash",

    "arrow", "plus-plus", "minus-minus", "star-equals",
    "slash-equals","minus-equals","plus-equals",
    "greater-than-equals","less-than-equals","equals-equals",

    "true","false","string","char","number","identifier",

    "bool","break","case","char","continue","do","double","else",
    "enum","extern","float","for","if","int","return","sizeof",
    "static","struct","switch","typedef","union","void","while",

    "semicolon","eof"
};

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: cc [file]\n");
        exit(1);
    }

    FILE* fp = fopen(argv[1], "rt");

    char buf[5000];
    int read = fread(buf, sizeof(char), 5000, fp);
    buf[read + 1] = 0;

    fclose(fp);

    initScanner(buf);
    
    Token t;
    char printBuf[100];
    for (t = scanNext(); t.type != TOK_EOF; t = scanNext()) {
        memcpy(printBuf, t.start, t.length);
        printBuf[t.length + 1] = 0;
        printf("Text: '%s'\nScanned as: '%s'\n\n", printBuf, names[t.type]);
    }

    return 0;
}