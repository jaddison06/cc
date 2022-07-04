#include "tests.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "scanner.h"
#include "array.h"

static char* module;

// slightly yucky but we move
#define expect(expression) _expect(expression, #expression)
#define expectStr(expression, expected) _expectStr(expression, #expression, expected)
#define expectNStr(expression, length, expected) _expectNStr(expression, #expression, length, expected)

static void _expect(bool expression, char* expressionString) {
    static char* oldModule = NULL;
    static int id;
    if (
        oldModule == NULL ||
        strcmp(oldModule, module) != 0
    ) {
        oldModule = module;
        id = 1;
        printf("\nTesting module '%s':\n", module);
    }

    if (!expression) {
        printf("\033[0;31mTest %i (%s) failed\033[0m\n", id++, expressionString);
        exit(70);
    } else {
        printf("\033[0;32mTest %i passed\033[0m\n", id++);
    }
}

static void _expectStr(char* expression, char* expressionStr, char* expected) {
    char* buf = malloc(strlen(expressionStr) + strlen(expression) + strlen(expected) + 13);
    sprintf(buf, "%s -> '%s' == '%s'", expressionStr, expression, expected);
    _expect(strcmp(expression, expected) == 0, buf);
    free(buf);
}

static void _expectNStr(char* expression, char* expressionStr, int length, char* expected) {
    char* buf = malloc(length + 1);
    strncpy(buf, expression, length);
    buf[length] = 0;
    int lengthStrLen = snprintf(NULL, 0, "%d", length);
    char* newExpressionStr = malloc(strlen(expressionStr) + lengthStrLen + 3);
    sprintf(newExpressionStr, "%s :%d", expressionStr, length);
    _expectStr(buf, newExpressionStr, expected);
    free(newExpressionStr);
    free(buf);
}

void testScanner() {
    initScanner("int test()[]!=!++");
    expect(scanNext().type == TOK_INT);
    Token tok = scanNext();
    expect(tok.type == TOK_IDENTIFIER);
    expectNStr(tok.start, tok.length, "test");
    expect(scanNext().type == TOK_LPAREN);
    expect(scanNext().type == TOK_RPAREN);
    expect(scanNext().type == TOK_LSQUARE);
    expect(scanNext().type == TOK_RSQUARE);
    expect(scanNext().type == TOK_BANGEQ);
    expect(scanNext().type == TOK_BANG);
    expect(scanNext().type == TOK_PLUSPLUS);
    expect(scanNext().type == TOK_EOF);
}

typedef struct {
    int* start;
    int capacity;
    int length;
} ArrayTest;

void testArray() {
    ArrayTest test = {
        .start = malloc(sizeof(int)),
        .capacity = 1,
        .length = 0
    };
    int i = 3;
    APPEND(test.start, i, test.length, test.capacity);
    expect(test.length == 1);
    expect(test.capacity == 1);
    expect(test.start[0] == 3);
    i = 10;
    APPEND(test.start, i, test.length, test.capacity);
    expect(test.length == 2);
    expect(test.capacity == 2);
    expect(test.start[0] == 3);
    expect(test.start[1] == 10);
    i = 6;
    APPEND(test.start, i, test.length, test.capacity);
    expect(test.length == 3);
    expect(test.capacity == 4);
    expect(test.start[0] == 3);
    expect(test.start[1] == 10);
    expect(test.start[2] == 6);
    
    free(test.start);
}

void testAll() {
    module = "scanner";
    testScanner();
    module = "array";
    testArray();
}