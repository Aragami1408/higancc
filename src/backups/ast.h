#ifndef HIGANCC_AST_H
#define HIGANCC_AST_H

#include "token.h"
#include "memory.h"
#include "arraylist.h"

typedef struct ASTProgram ASTProgram;
typedef struct ASTFunction ASTFunction;
typedef struct ASTStatement ASTStatement;
typedef struct ASTExpression ASTExpression;

ARRAYLIST_PROTOTYPE(ASTFunction)
ARRAYLIST_PROTOTYPE(ASTStatement)
ARRAYLIST_PROTOTYPE(ASTExpression)

typedef enum {
	AST_STATEMENT_RETURN,
} ASTStatementType;

typedef enum {
	AST_EXPRESSION_CONSTANT,
	AST_EXPRESSION_UNARY,
} ASTExpressionType;

typedef enum {
	AST_UNARY_COMPLEMENT,
	AST_UNARY_NEGATE
} ASTOperator;

struct ASTProgram {
	ArrayList(ASTFunction) *functions;
	ArenaAllocator *allocator;
};

struct ASTFunction {
	const char *name;
	ArrayList(ASTStatement) *statements;
	ArenaAllocator *allocator;
};

struct ASTStatement {
	ASTStatementType type;
	union {
		ASTExpression *return_expr;
	};
};

struct ASTExpression {
	ASTExpressionType type;
	union {
		int constant;
		struct {
			ASTOperator op;
			ASTExpression *val;
		} unary;
	};
};

ASTProgram *AST_createProgram(ArenaAllocator *a);
ASTFunction *AST_createFunction(const char *name, ArenaAllocator *a);
ASTExpression *AST_createExpression(ASTExpressionType type, ArenaAllocator *a);

#endif
