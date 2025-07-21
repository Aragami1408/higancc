#include "ast.h"


ARRAYLIST_IMPL(ASTFunction)
ARRAYLIST_IMPL(ASTStatement)
ARRAYLIST_IMPL(ASTExpression)


ASTProgram *AST_createProgram(ArenaAllocator *a) {
	ASTProgram *program = (ASTProgram*)ArenaAllocator_alloc(a, sizeof(ASTProgram));
	program->functions = ArrayList_init(ASTFunction, a);
    program->allocator = a;
    return program;
}

ASTFunction *AST_createFunction(const char *name, ArenaAllocator *a) {
	ASTFunction *function = (ASTFunction*)ArenaAllocator_alloc(a, sizeof(ASTFunction));
	function->name = name;
	function->statements = ArrayList_init(ASTStatement, a);	
	function->allocator = a;
	return function;
}

ASTExpression *AST_createExpression(ASTExpressionType type, ArenaAllocator *a) { 
	ASTExpression *expression = (ASTExpression*)ArenaAllocator_alloc(a, sizeof(ASTExpression));
	expression->type = type;
	return expression;
}
