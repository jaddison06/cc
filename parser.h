#pragma once

#include <stdbool.h>

#include "scanner.h"

// forward decl >:(
typedef struct Expression Expression;
typedef struct Statement Statement;

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
    Expression* operand;
    Token operator;
} ExprUnary;

typedef struct {
    Expression* a, * b;
    Token operator;
} ExprBinary;

typedef struct {
    Expression* condition;
    Expression* a, * b;
} ExprTernary;

typedef struct {
    Expression* object;
    Token field;
} ExprGet;

typedef struct {
    Expression* callable;
    Expression* argsRoot;
    int argsLen, argsCap;
} ExprCall;

struct Expression {
    ExpressionType type;
    union {
        ExprUnary unary;
        ExprBinary binary;
        ExprTernary ternary;
        ExprCall call;
        ExprGet get;
    };
};

typedef struct {
    Type type;
    Token name;
    bool initialized;
    Expression initializer;
} VarDecl;

typedef enum {
    BLOCK_VARDECL,
    BLOCK_STMT
} BlockElemType;

typedef struct {
    BlockElemType type;
    union {
        VarDecl varDecl;
        Statement* statement;
    };
} BlockElement;

typedef struct {
    BlockElement* root;
    int len, cap;
} Block;

typedef enum {
    FOR_DECL,
    FOR_EXPR,
    FOR_NONE
} ForExprInitType;

typedef struct {
    ForExprInitType initType;
    union {
        VarDecl initDecl;
        Expression initExpr;
    };
    bool hasCond;
    Expression cond;
    bool hasIncr;
    Expression incr;
} StmtFor;

typedef struct {
    Expression cond;
    Statement* body;
    bool hasElse;
    Statement* elseBranch;
} StmtIf;

typedef struct {
    Expression cond;
    Statement* body;
} StmtWhile;

typedef struct {
    bool hasValue;
    Expression value;
} StmtReturn;

typedef struct {
    bool isDefault;
    Expression cond;
    Statement* body;
} SwitchBranch;

typedef struct {
    SwitchBranch* root;
    int len, cap;
} StmtSwitch;

typedef enum {
    STMT_EXPR,
    STMT_FOR,
    STMT_IF,
    STMT_WHILE,
    STMT_DOWHILE,
    STMT_RETURN,
    STMT_SWITCH,
    STMT_BREAK,
    STMT_CONTINUE,
    STMT_BLOCK
} StmtType;

struct Statement {
    StmtType type;
    union {
        Expression expr;
        StmtFor for_;
        StmtIf if_;
        // also used for do-while
        StmtWhile while_;
        StmtReturn return_;
        StmtSwitch switch_;
        Block block;
    };
};

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
void DestroyFile(File* file);