#pragma once

typedef enum {
    // symbols and shit
    TOK_LBRACE, TOK_RBRACE, TOK_LSQUARE, TOK_RSQUARE, TOK_LPAREN, TOK_RPAREN,
    TOK_COMMA, TOK_DOT, TOK_STAR, TOK_SLASH, TOK_MINUS, TOK_PLUS, TOK_GREATER,
    TOK_LESS, TOK_EQ, TOK_HASH,

    TOK_ARROW, TOK_PLUSPLUS, TOK_MINUSMINUS, TOK_STAREQ, TOK_SLASHEQ, TOK_MINUSEQ,
    TOK_PLUSEQ, TOK_GREATEREQ, TOK_LESSEQ, TOK_EQEQ,

    // literals n that
    TOK_TRUE, TOK_FALSE, TOK_STRINGLIT, TOK_CHARLIT, TOK_NUMLIT, TOK_IDENTIFIER,

    // keywords - not exhaustive but im not implementing fucking gotos
    TOK_BOOL, TOK_BREAK, TOK_CASE, TOK_CHAR, TOK_CONTINUE, TOK_DO, TOK_DOUBLE, TOK_ELSE,
    TOK_ENUM, TOK_EXTERN, TOK_FLOAT, TOK_FOR, TOK_IF, TOK_INT, TOK_RETURN, TOK_SIZEOF,
    TOK_STATIC, TOK_STRUCT, TOK_SWITCH, TOK_TYPEDEF, TOK_UNION, TOK_VOID, TOK_WHILE,

    TOK_SEMICOLON, TOK_EOF
} TokenType;

typedef struct {
    TokenType type;
    char* start;
    int length;
} Token;

void initScanner(char* source);
Token scanNext();