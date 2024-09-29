#include "dump.h"
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

void dump_ast(const struct ASTProgram *program) {
	usize depth = 0;

	printf("Program {\n");

	for (usize i = 0; i < program->functions->len; i++) {
		ASTFunction func = ArrayList_get(ASTFunction, program->functions, i);
		print_ast_function(&func, depth+1);
	}

	printf("}\n");	
}

static void print_tacky_val(const TackyVal *val) {
	switch (val->type) {
		case TACKY_VAL_CONSTANT:
			printf("Constant(%d)", val->constant);
			break;
		case TACKY_VAL_VARIABLE:
			printf("Variable(%s)", val->variable);
			break;
	}
}

static void print_tacky_instruction(const TackyInstruction *instruction, usize depth) {
	TAB;

	switch (instruction->type) {
        case TACKY_INSTRUCTION_RETURN:
			printf("Return(");
			print_tacky_val(&instruction->return_val);
        break;
        case TACKY_INSTRUCTION_UNARY:
			printf("Unary(");
			switch (instruction->unary.op) {
				case TACKY_OPERATOR_UNARY_COMPLEMENT:
					printf("Complement, ");
					break;
				case TACKY_OPERATOR_UNARY_NEGATE:
					printf("Negate, ");
					break;
			}
			print_tacky_val(&instruction->unary.src);
			printf(", ");
			print_tacky_val(&instruction->unary.dst);
        break;
        }

	printf(")\n");	
}

static void print_tacky_function(const TackyFunction *func, usize depth) {
	TAB;
	printf("Function[%s] {\n", func->name);

	for (usize i = 0; i < func->body->len; i++) {
		TackyInstruction instruction = ArrayList_get(TackyInstruction, func->body, i);
		print_tacky_instruction(&instruction, depth+1);
	}

	TAB;
	printf("}\n");	
}

void dump_tacky(const TackyProgram *program) {
	usize depth = 0;

	printf("Program {\n");
	for (usize i = 0; i < program->functions->len; i++) {
		TackyFunction func = ArrayList_get(TackyFunction, program->functions, i);
		print_tacky_function(&func, depth+1);
	}
	printf("}\n");
}
