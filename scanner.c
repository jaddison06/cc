#include "scanner.h"

#include <stdbool.h>
#include <string.h>

static char* start;
static char* current;

void initScanner(char* source) {
    start = current = source;
}

static bool isAtEnd() {
    return current == 0;
}

static char advance() {
    return *current++;
}

static char peek() {
    return *current;
}

static char peekNext() {
    if (isAtEnd()) return 0;
    return current[1];
}

static void skipWhitespace() {
    for (;;) {
        switch (peek()) {
            case ' ':
            case '\r':
            case '\t':
            case '\n':
                advance();
                break;
            case '/':
                if (peekNext() == '/') {
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else if (peekNext() == '*') {
                    while (!(peek() == '*' && peekNext() != '/') && !isAtEnd) advance();
                    advance();
                } else
                    return;
                break;
            default:
                return;
        }
    }
}

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isAlpha(char c) {
    return
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_';
}

static bool match(char expected) {
    if (isAtEnd()) return false;
    if (peek() != expected) return false;
    current++;
    return true;
}

static Token makeTok(TokenType type) {
    return (Token){
        .type = type,
        .start = start,
        .length = current - start 
    };
}

static TokenType checkKeyword(int kwStart, char* rest, TokenType type) {
    int length = strlen(rest);
    if (
        current - start == kwStart + length &&
        strncmp(start + kwStart, rest, length) == 0
    ) return type;

    return TOK_IDENTIFIER;
}

static Token identifier() {
    while (isAlpha(peek()) || isDigit(peek())) advance();
    switch (start[0]) {
        case 'r': return makeTok(checkKeyword(1, "eturn", TOK_RETURN));
        case 't': return makeTok(checkKeyword(1, "ypedef", TOK_TYPEDEF));
        case 'u': return makeTok(checkKeyword(1, "nion", TOK_UNION));
        case 'v': return makeTok(checkKeyword(1, "oid", TOK_VOID));
        case 'w': return makeTok(checkKeyword(1, "hile", TOK_WHILE));

        case 'b':
            if (current - start > 1) {
                switch (start[1]) {
                    case 'o': return makeTok(checkKeyword(2, "ol", TOK_BOOL));
                    case 'r': return makeTok(checkKeyword(2, "eak", TOK_BREAK));
                }
            }
            break;
        case 'c':
            if (current - start > 1) {
                switch (start[1]) {
                    case 'a': return makeTok(checkKeyword(2, "se", TOK_CASE));
                    case 'h': return makeTok(checkKeyword(2, "ar", TOK_CHAR));
                    case 'o': return makeTok(checkKeyword(2, "ntinue", TOK_CONTINUE));
                }
            }
            break;
        case 'd':
            if (current - start == 1 && start[1] == 'o')
                return makeTok(TOK_DO);
            
            if (current - start > 2)
                return makeTok(checkKeyword(2, "ouble", TOK_DOUBLE));
            
            break;
        case 'e':
            if (current - start > 1) {
                switch (start[1]) {
                    case 'l': return makeTok(checkKeyword(2, "se", TOK_ELSE));
                    case 'n': return makeTok(checkKeyword(2, "um", TOK_ENUM));
                    case 'x': return makeTok(checkKeyword(2, "tern", TOK_EXTERN));
                }
            }
            break;
        case 'f':
            if (current - start > 1) {
                switch (start[1]) {
                    case 'l': return makeTok(checkKeyword(2, "oat", TOK_FLOAT));
                    case 'o': return makeTok(checkKeyword(2, "r", TOK_FOR));
                }
            }
            break;
        case 'i':
            if (current - start == 1 && start[1] == 'f')
                return makeTok(TOK_IF);
            
            if (current - start > 1)
                return makeTok(checkKeyword(1, "nt", TOK_INT));
            break;
        case 's':
            if (current - start > 1) {
                switch (start[1]) {
                    case 'i': return makeTok(checkKeyword(2, "zeof", TOK_SIZEOF));
                    case 'w': return makeTok(checkKeyword(2, "itch", TOK_SWITCH));
                    case 't':
                        if (current - start > 2) {
                            switch (start[2]) {
                                case 'a': return makeTok(checkKeyword(3, "tic", TOK_STATIC));
                                case 'r': return makeTok(checkKeyword(3, "uct", TOK_STRUCT));
                            }
                        }
                }
            }
            break;
    }

    return makeTok(TOK_IDENTIFIER);
}

static Token number() {
    while (isDigit(peek())) advance();
    if (peek() == '.' && isDigit(peekNext())) {
        advance();
        while (isDigit(peek())) advance();
    }

    return makeTok(TOK_NUMLIT);
}

// todo: escape chars - process here? or in parser

static Token character() {
    // todo: error check here
    while (peek() != '\'' && !isAtEnd())
        advance();
    
    // todo: error here
    if (isAtEnd());

    advance();
    return makeTok(TOK_CHARLIT);
}

static Token string() {
    while (peek() != '"' && !isAtEnd())
        advance();
    
    // todo: error here
    if (isAtEnd());

    advance();
    return makeTok(TOK_STRINGLIT);
}

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) > (b) ? (b) : (a))

Token scanNext() {
    skipWhitespace();

    start = current;
    if (isAtEnd()) return makeTok(TOK_EOF);

    char c = advance();

    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();

    switch (c) {
        case '{': return makeTok(TOK_LBRACE);
        case '}': return makeTok(TOK_RBRACE);
        case '[': return makeTok(TOK_LSQUARE);
        case ']': return makeTok(TOK_RSQUARE);
        case '(': return makeTok(TOK_LPAREN);
        case ')': return makeTok(TOK_RPAREN);
        case '#': return makeTok(TOK_HASH);

        case ',': return makeTok(TOK_COMMA);
        case '.': return makeTok(TOK_DOT);
        case ';': return makeTok(TOK_SEMICOLON);

        case '*': return makeTok(
            match('=') ? TOK_STAREQ : TOK_STAR
        );
        case '/': return makeTok(
            match('=') ? TOK_SLASHEQ : TOK_SLASH
        );
        case '+': return makeTok(
            match('=') ? TOK_PLUSEQ :
            match('+') ? TOK_PLUSPLUS : TOK_PLUS
        );
        case '-': return makeTok(
            match('=') ? TOK_MINUSEQ :
            match('-') ? TOK_MINUSMINUS :
            match('>') ? TOK_ARROW : TOK_MINUS
        );
        case '>': return makeTok(
            match('=') ? TOK_GREATEREQ : TOK_GREATER
        );
        case '<': return makeTok(
            match('=') ? TOK_LESSEQ : TOK_LESS
        );
        case '=': return makeTok(
            match('=') ? TOK_EQEQ : TOK_EQ
        );
        case '!': return makeTok(
            match('=') ? TOK_BANGEQ : TOK_BANG
        );

        case '\'': return character();
        case '"': return string();
    }
}