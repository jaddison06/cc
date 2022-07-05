#pragma once

#include <stdbool.h>

#include "scanner.h"

typedef struct {
    Token typename;
    int pointerDepth;
} Type;

typedef struct {
    Token* root;
    int len, cap;
} Preprocessor;

typedef enum {
    EXPR_PREFIX,
    EXPR_POSTFIX,
    EXPR_BINARY,
    EXPR_TERNARY,
    EXPR_GET,
    EXPR_CALL
} ExpressionType;

// all expression subtypes use Expression for operands etc,
// assuming that the parser will sort out precedence

typedef struct {
    Expression operand;
    Token operator;
} ExprUnary;

typedef struct {
    Expression a, b;
    Token operator;
} ExprBinary;

typedef struct {
    Expression condition;
    Expression a, b;
} ExprTernary;

typedef struct {
    Expression object;
    Token field;
} ExprGet;

typedef struct {
    Expression callable;
    Expression* argsRoot;
    int argsLen, argsCap;
} ExprCall;

typedef struct {
    ExpressionType type;
    union {
        ExprUnary unary;
        ExprBinary binary;
        ExprTernary ternary;
        ExprCall call;
        ExprGet get;
    };
} Expression;

typedef struct {

} Statement;

typedef enum {
    BLOCK_VARDECL,
    BLOCK_STMT
} BlockElemType;

typedef struct {
    BlockElemType type;
    union {
        VarDecl varDecl;
        Statement statement;
    };
} BlockElement;

typedef struct {
    BlockElement* root;
    int len, cap
} Block;

typedef struct {
    Type type;
    Token name;
    bool initialized;
    Expression initializer;
} VarDecl;

typedef struct {
    Type type;
    Token name;
    // assume every param is named
    Type* paramTypes;
    Token* paramNames;
    int paramsLen, paramsCap;
    Block contents;
} FunctionDecl;

typedef enum {
    DECL_FUN,
    DECL_VAR
} DeclType;

typedef struct {
    DeclType type;
    union {
        FunctionDecl fun;
        VarDecl var;
    };
} Declaration;

typedef enum {
    TOP_PP, // preprocessor
    TOP_DECL
} TopLevelType;

typedef struct {
    TopLevelType type;
    union {
        Preprocessor preprocessor;
        Declaration declaration;
    };
} TopLevel;

typedef struct {
    TopLevel* root;
    int len, cap;
} File;

File compile(char* source);