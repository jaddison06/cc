#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "scanner.h"

#if CC_RUN_TESTS
#include "tests.h"
#endif

static char* names[] = {
    "lbrace","rbrace","lsquare","rsquare",
    "lparen","rparen","comma","dot","star",
    "slash","minus","plus","greater-than",
    "less-than","equals","hash","bang","pipe",
    "caret","ampersand","modulo","tilde",

    "arrow", "plus-plus", "minus-minus", "star-equals",
    "slash-equals","minus-equals","plus-equals",
    "greater-than-equals","less-than-equals","equals-equals",
    "bang-equals","left-shift","right-shift","pipe-pipe",
    "ampersand-ampersand","pipe-equals","caret-equals","ampersand-equals",
    "modulo-equals","tilde-tilde",

    "left-shift-equals","right-shift-equals","ampersand-ampersand-equals",
    "pipe-pipe-equals",

    "true","false","string","char","number","identifier",

    "bool","break","case","char","continue","do","default","double",
    "else","enum","extern","float","for","if","int","return","sizeof",
    "static","struct","switch","typedef","union","void","while",

    "semicolon","eof",

    "error"
};

char* readFile(char* fname) {
    FILE* fp = fopen(fname, "rb");

    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);
    rewind(fp);

    char* out = malloc(fileSize + 1);

    fread(out, sizeof(char), fileSize, fp);

    out[fileSize] = 0;

    fclose(fp);

    return out;
}

int main(int argc, char** argv) {
#if CC_RUN_TESTS
    testAll();
#else
    if (argc != 2) {
        printf("Usage: cc [file]\n");
        exit(1);
    }

    char* buf = readFile(argv[1]);

    initScanner(buf);

    FILE* fp = fopen("toks.txt", "wt");
    
    Token t;
    char* writeBuf;
    int bufLen = 0;
    for (t = scanNext(); t.type != TOK_EOF; t = scanNext()) {
        if (t.length >= bufLen) {
            writeBuf = realloc(writeBuf, t.length + 1);
            bufLen = t.length + 1;
        }
        memcpy(writeBuf, t.start, t.length);
        writeBuf[t.length] = 0;
        // printf("Text: '%s'\nScanned as: '%s'\n\n", writeBuf, names[t.type]);
        fprintf(fp, "%s\n", writeBuf);
    }

    free(writeBuf);

    fclose(fp);

    free(buf);

#endif
    return 0;
}