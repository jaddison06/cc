#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "scanner.h"

static char* names[] = {
    "lbrace","rbrace","lsquare","rsquare",
    "lparen","rparen","comma","dot","star",
    "slash","minus","plus","greater-than",
    "less-than","equals","hash","bang",

    "arrow", "plus-plus", "minus-minus", "star-equals",
    "slash-equals","minus-equals","plus-equals",
    "greater-than-equals","less-than-equals","equals-equals",
    "bang-equals",

    "true","false","string","char","number","identifier",

    "bool","break","case","char","continue","do","double","else",
    "enum","extern","float","for","if","int","return","sizeof",
    "static","struct","switch","typedef","union","void","while",

    "semicolon","eof",

    "error"
};

char* readFile(char* fname) {
    FILE* fp = fopen(fname, "rt");

    fseek(fp, 0, SEEK_END);
    int fileSize = ftell(fp);
    rewind(fp);

    char* out = malloc(fileSize + 1);

    fread(out, sizeof(char), fileSize, fp);

    out[fileSize] = 0;

    fclose(fp);

    return out;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: cc [file]\n");
        exit(1);
    }

    char* buf = readFile(argv[1]);

    initScanner(buf);

    FILE* fp = fopen("toks.txt", "wt");
    
    Token t;
    char writeBuf[100];
    for (t = scanNext(); t.type != TOK_EOF; t = scanNext()) {
        memcpy(writeBuf, t.start, t.length);
        writeBuf[t.length] = 0;
        // printf("Text: '%s'\nScanned as: '%s'\n\n", printBuf, names[t.type]);
        fprintf(fp, "%s\n", writeBuf);
    }

    fclose(fp);

    free(buf);

    return 0;
}