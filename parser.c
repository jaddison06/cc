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

    Type out = {.typename = previous, .pointerDepth = 0};
    while (match(TOK_STAR)) out.pointerDepth++;
    return out;
}

// needs-impl
static Statement statement() {

}

// start from token after lparen
static FunctionDecl function() {
    FunctionDecl out;

    INIT(out.params);
    
    if (!check(TOK_RPAREN)) {
        Param currentParam;
        do {
            currentParam.type = getType();
            currentParam.name = consume(TOK_IDENTIFIER, "Expected parameter name");
            APPEND(out.params, currentParam);
        } while (match(TOK_COMMA));
    }

    out.body = statement();

    return out;
}

// start from first token after name & type
// needs-impl
static VarDecl variableDecl() {
    if (match(TOK_LSQUARE)) {
            // array
        }
        if (match(TOK_EQ)) {
            // assignment
        }
        consume(TOK_SEMICOLON, "Expected semicolon after variable declaration");
}

static Declaration declaration() {
    Declaration out;

    Type type = getType();
    Token name = consume(TOK_IDENTIFIER, "Expected an identifier");
    if (match(TOK_LPAREN)) {
        out.tag = DECL_FUN;
        out.fun = function();
    } else {
        out.tag = DECL_VAR;
        out.var = variableDecl();
    }
    //! See Declaration typedef in parser.h for details
    //! and help when the inevitable memory bug occurs
    out.type = type;
    out.name = name;

    return out;
}

// needs-impl
static Preprocessor preprocessor() {

}

File compile(char* source) {
    initScanner(source);

    File out;

    while (!match(TOK_EOF)) {
        TopLevel tl;

        if (match(TOK_HASH))
            tl = (TopLevel){.tag = TOP_PP, .preprocessor = preprocessor()};
        else
            tl = (TopLevel){.tag = TOP_DECL, .declaration = declaration()};
        
        APPEND(out, tl);
    }
}

void DestroyFile(File* file) {
    
}