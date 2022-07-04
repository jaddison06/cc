#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"

static Token previous;
static Token current;

static void error(char* msg) {
    printf("\033[0;31m%*.*s: %s\033[0m\n", current.length, current.length, current.start, msg);
    exit(1);
}

static void advance() {
    previous = current;
    current = scanNext();
}

static Token consume(TokenType type, char* msg) {
    if (current.type == type) {
        advance();
        return previous;
    }

    error(msg);
}

static bool check(TokenType type) {
    return current.type == type;
}

static bool match(TokenType type) {
    if (!check(type)) return false;
    advance();
    return true;
}

static Type getType() {
    if (!(
        match(TOK_INT) |
        match(TOK_DOUBLE) |
        match(TOK_FLOAT) |
        match(TOK_CHAR) |
        match(TOK_BOOL) |
        match(TOK_IDENTIFIER)
    )) error("Expected a typename");

    Type out = {.typename = current, .pointerDepth = 0};
    while (match(TOK_STAR)) out.pointerDepth++;
    return out;
}

static void declaration() {
    Type type = getType();
    Token name = consume(TOK_IDENTIFIER, "Expected an identifier");
    if (match(TOK_LPAREN)) {
        // function
    } else {
        // variable
        if (match(TOK_LSQUARE)) {
            // array
        }
        if (match(TOK_EQ)) {
            // assignment
        }
        consume(TOK_SEMICOLON, "Expected semicolon after variable declaration");
    }
}

static void preprocessor() {

}

void compile(char* source) {
    initScanner(source);

    advance();

    while (!match(TOK_EOF)) {
        if (match(TOK_HASH)) preprocessor();
        else declaration();
    }
}