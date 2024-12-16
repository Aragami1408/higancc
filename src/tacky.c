#include "tacky.h"
#include "utils.h"

ARRAYLIST_IMPL(TackyProgram);
ARRAYLIST_IMPL(TackyFunction);
ARRAYLIST_IMPL(TackyInstruction);

static TackyVal make_tmp(Tacky *tacky) {
	TackyVal tmp;
	tmp.type = TACKY_VAL_VARIABLE;
	tmp.variable = format_string("tmp%d", tacky->tmp_counter);
	tacky->tmp_counter += 1;
	return tmp;
}


static TackyVal emit_expr(Tacky *tacky, const ASTExpression *expression) {
	switch (expression->type) {
		case AST_EXPRESSION_CONSTANT:
			return (TackyVal) {.type = TACKY_VAL_CONSTANT, .constant = expression->constant};
		case AST_EXPRESSION_UNARY: {
			TackyVal val = emit_expr(tacky, expression->unary.val);
			TackyVal dst = make_tmp(tacky);
			TackyOperator op;
			switch (expression->unary.op) {
				case AST_UNARY_COMPLEMENT:
					op = TACKY_OPERATOR_UNARY_COMPLEMENT;
					break;
				case AST_UNARY_NEGATE:
					op = TACKY_OPERATOR_UNARY_NEGATE;
					break;
			}

			TackyInstruction tacky_unary;
			tacky_unary.type = TACKY_INSTRUCTION_UNARY;
			tacky_unary.unary.op = op;
			tacky_unary.unary.src = val;
			tacky_unary.unary.dst = dst;

			ArrayList_add(TackyInstruction, tacky->instructions, tacky_unary);
			
			return dst;
		}
		default:
			return (TackyVal) {.type = TACKY_VAL_CONSTANT, .constant = -1};
	}
}

static void emit_statement(Tacky *tacky, const ASTStatement *statement) {

	switch (statement->type) {
		case AST_STATEMENT_RETURN: {
			TackyInstruction ret;
			ret.type = TACKY_INSTRUCTION_RETURN;
			ret.return_val = emit_expr(tacky, statement->return_expr);
			ArrayList_add(TackyInstruction, tacky->instructions, ret);
		}
	}

}

static ArrayList(TackyInstruction) *emit_instructions(Tacky *tacky, ArrayList(ASTStatement) *statements) {
	if (statements == NULL) {
		// TODO(higanbana): Proper error handling needed
		return NULL;
	}

	for (usize i = 0; i < ArrayList_size(ASTStatement, statements); i++) {
		ASTStatement statement = ArrayList_get(ASTStatement, statements, i);
		emit_statement(tacky, &statement);
    }

	return tacky->instructions;
}

Tacky *Tacky_create(ArenaAllocator *allocator) {
	Tacky *tacky = (Tacky *)ArenaAllocator_alloc(allocator, sizeof(Tacky));
	tacky->instructions = ArrayList_init(TackyInstruction, allocator);
	tacky->tmp_counter = 0;
	tacky->allocator = allocator;
	return tacky;
}

TackyProgram *emit_tacky(Tacky *tacky, const ASTProgram *program) {
	if (program->functions == NULL) {
		// TODO(higanbana): Proper error handling needed
		return NULL;
	}

	TackyProgram *tacky_program= (TackyProgram *)ArenaAllocator_alloc(tacky->allocator, sizeof(TackyProgram));
	ArrayList(TackyFunction) *tacky_functions = ArrayList_init(TackyFunction, tacky->allocator);

	for (usize i = 0; i < ArrayList_size(ASTFunction, program->functions); i++) {
		ASTFunction function = ArrayList_get(ASTFunction, program->functions, i);
		if (function.statements == NULL) {
			// TODO(higanbana): Proper error handling needed
			return NULL;
		}

		TackyFunction tacky_function;
		tacky_function.name = function.name;
		tacky_function.body = emit_instructions(tacky, function.statements);
		
		ArrayList_add(TackyFunction, tacky_functions, tacky_function);
	}
	tacky_program->functions = tacky_functions;
	return tacky_program;	
}
