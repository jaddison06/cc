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
} ExpressionTag;

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
    ExpressionTag tag;
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
    BlockElemType tag;
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
    ForExprInitType initTag;
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
} StmtTag;

struct Statement {
    StmtTag tag;
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
} Param;
DECL_VEC(Param, ParamList);

typedef struct {
    Type type;
    Token name;
    // assume every param is named
    ParamList params;
    Statement body;
} FunctionDecl;

typedef enum {
    DECL_FUN,
    DECL_VAR
} DeclTag;

typedef struct {
    DeclTag tag;
    union {
        FunctionDecl fun;
        VarDecl var;
        //* risky - i'm like 75% sure that this'll let you set both,
        //* because FunctionDecl and VarDecl have type & name as their
        //* first members
        struct {
            Type type;
            Token name;
        };
    };
} Declaration;

typedef enum {
    TOP_PP, // preprocessor
    TOP_DECL
} TopLevelTag;

typedef struct {
    TopLevelTag tag;
    union {
        Preprocessor preprocessor;
        Declaration declaration;
    };
} TopLevel;


DECL_VEC(TopLevel, File);

File compile(char* source);
void DestroyFile(File* file);