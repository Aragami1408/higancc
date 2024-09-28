#include "pretty_print.h"
#include "common.h"

#define TAB \
	for (usize i = 0; i < depth; i++) printf("  ");

static void print_ast_expression(const ASTExpression *expr, usize depth) {
	TAB;
	switch (expr->type) {
        case AST_EXPRESSION_CONSTANT:
			printf("Constant(%d)\n", expr->constant);
			break;
        case AST_EXPRESSION_UNARY:
			printf("Unary(");
			switch (expr->unary.op) {
				case AST_UNARY_COMPLEMENT:
					printf("~,\n");
					break;
				case AST_UNARY_NEGATE:
					printf("-,\n");
					break;
			}
			print_ast_expression(expr->unary.val, depth+1);
        	break;
    }
}

static void print_ast_statement(const ASTStatement *stmt, usize depth) {
	TAB;

	switch (stmt->type) {
        case AST_STATEMENT_RETURN:
			printf("Return {\n");
			print_ast_expression(stmt->return_expr, depth+1);
        break;
    }

	TAB;
	printf("}\n");	
}

static void print_ast_function(const ASTFunction *func, usize depth) {
	TAB;
	printf("Function[%s] {\n", func->name);

	for (usize i = 0; i < func->statements->len; i++) {
		ASTStatement statement = ArrayList_get(ASTStatement, func->statements, i);
		print_ast_statement(&statement, depth+1);
	}

	TAB;
	printf("}\n");	
}



void pretty_print_ast(const struct ASTProgram *program) {
	usize depth = 0;

	printf("Program {\n");

	for (usize i = 0; i < program->functions->len; i++) {
		ASTFunction func = ArrayList_get(ASTFunction, program->functions, i);
		print_ast_function(&func, depth+1);
	}

	printf("}\n");	
}
