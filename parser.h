#pragma once

#include <stdbool.h>

#include "scanner.h"
#include "vector.h"

// forward decl >:(
typedef struct Expression Expression;
typedef struct Statement Statement;

typedef struct {
    Token typename;
    int pointerDepth;
} Type;

DECL_VEC(Token, Preprocessor)

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

DECL_VEC(Expression, Arguments)

typedef struct {
    Expression* callable;
    Arguments arguments;
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

DECL_VEC(BlockElement, Block)

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

DECL_VEC(SwitchBranch, StmtSwitch)

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

DECL_VEC(struct {Type type; Token name;}, ParamList)

typedef struct {
    Type type;
    Token name;
    // assume every param is named
    ParamList params;
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


DECL_VEC(TopLevel, File);

File compile(char* source);
void DestroyFile(File* file);