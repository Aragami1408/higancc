#include <stdarg.h>

#include "parser.h"
#include "ast.h"
#include "utils.h"

// HELPER FUNCTIONS
// ----------------

static Token peek(const Parser *parser) {
	return parser->tokens->data[parser->current];
}

static Token previous(const Parser *parser) {
	return parser->tokens->data[parser->current - 1];
}

static bool is_at_end(const Parser *parser) {
	return peek(parser).type == TOKEN_EOF;
}

// TODO(higanbana): Error should return something rather than void
static void error(const Parser *parser, const char *msg) {
	Token current_token = peek(parser);
	fprintf(stderr, "[PARSER ERROR - line %d] %s\n", current_token.line, msg);
	exit(1);
}

static Token advance(Parser *parser) {
	if (!is_at_end(parser)) parser->current++;
	return previous(parser);
}

static bool check(const Parser *parser, TokenType type) {
	if (is_at_end(parser)) return false;
	return peek(parser).type == type;
}

static void consume(Parser *parser, TokenType type, const char *message) {
	if (!check(parser, type)) {
		error(parser, message);
	}
	advance(parser);
}

/*
   static bool match(Parser *parser, usize count, ...) {
   va_list args; 
   va_start(args, count);

   for (usize i = 0; i < count; i++) {
   TokenType type = va_arg(args, TokenType);
   if (check(parser, type)) {
   advance(parser);
   va_end(args);
   return true;
   }
   }

   va_end(args);
   return false;
   }
 */


// RECURSIVE DESCENT PARSING
// -------------------------

// static ASTProgram *parse_program(Parser *parser);
static ASTFunction *parse_function(Parser *parser, ArenaAllocator *allocator);
static ASTStatement *parse_statement(Parser *parser, ArenaAllocator *allocator);
static ASTExpression *parse_exp(Parser *parser, ArenaAllocator *allocator);
static ASTOperator parse_unop(Parser *parser);


static ASTFunction *parse_function(Parser *parser, ArenaAllocator *allocator) {

	if (!(check(parser, TOKEN_KW_INT) || check(parser, TOKEN_KW_FLOAT))) {
		error(parser, "Expect 'int' or 'float' before function name.");
	}
	advance(parser);

	if (!check(parser, TOKEN_IDENTIFIER)) {
		error(parser, "Expect identifier after return type");
	}
	const char *name = strndup(peek(parser).start, peek(parser).length);
	advance(parser);

	ASTFunction *function = AST_createFunction(name, allocator);

	consume(parser, TOKEN_LEFT_PAREN, "Expect '(' after function name");
	consume(parser, TOKEN_KW_VOID, "Expect 'void' after '('");
	consume(parser, TOKEN_RIGHT_PAREN, "Expect ')' after void");
	consume(parser, TOKEN_LEFT_BRACE, "Expect '{' after ')'");

	while (!check(parser, TOKEN_RIGHT_BRACE)) {
		ASTStatement *statement = parse_statement(parser, allocator);
		ArrayList_add(ASTStatement, function->statements, *statement);
	}

	advance(parser);
	
	return function;
}

static ASTStatement *parse_statement(Parser *parser, ArenaAllocator *allocator) {
	consume(parser, TOKEN_KW_RETURN, "Expect 'return' before expression.");

	ASTExpression *return_expr = parse_exp(parser, allocator);
	advance(parser);

	consume(parser, TOKEN_SEMICOLON, "Expect ';' after expression.");

	ASTStatement *statement = (ASTStatement *)ArenaAllocator_alloc(allocator, sizeof(ASTStatement));
	statement->return_expr = return_expr;
	return statement;
}

static ASTOperator parse_unop(Parser *parser) {
	if (check(parser, TOKEN_TILDE))
		return AST_UNARY_COMPLEMENT;
	else if (check(parser, TOKEN_MINUS))
		return AST_UNARY_NEGATE;
	else
		return -1;
}

static ASTExpression *parse_exp(Parser *parser, ArenaAllocator *allocator) {
	ASTExpression *expr = (ASTExpression *)ArenaAllocator_alloc(allocator, sizeof(ASTExpression));
	if (check(parser, TOKEN_INT) || check(parser, TOKEN_FLOAT)) {
		int value = atoi(substring(allocator, peek(parser).start, 0, peek(parser).length)); 
		expr->type = AST_EXPRESSION_CONSTANT;
		expr->constant = value;
		// return AST_createExp(parser->allocator, substring(parser->allocator, peek(parser).start, 0, peek(parser).length));
	}
	else if (check(parser, TOKEN_CHAR)) {
		char c = peek(parser).start[1];
		expr->type = AST_EXPRESSION_CONSTANT;
		expr->constant = (int)c;
	}
	else if (check(parser, TOKEN_TILDE) || check(parser, TOKEN_MINUS)) {
		ASTOperator operator = parse_unop(parser);
		advance(parser);
		ASTExpression *inner_exp = parse_exp(parser, allocator);
		expr->type = AST_EXPRESSION_UNARY;
		expr->unary.op= operator;
		expr->unary.val = inner_exp;
	}
	else if (check(parser, TOKEN_LEFT_PAREN)) {
		advance(parser);
		ASTExpression *inner_exp = parse_exp(parser, allocator);
		advance(parser);
		if (!check(parser, TOKEN_RIGHT_PAREN)) {
			error(parser, "Expect ')' after grouping expression");
		}
		return inner_exp;
	}
	else {
		error(parser, "Malformed expression");
		return NULL;
	}
	return expr;
}

// PUBLIC METHODS
// --------------
Parser Parser_init(ArrayList(Token) *tokens) {
	Parser parser;

	parser.tokens = tokens;
	parser.current = 0;

	return parser;
}

ASTProgram *Parser_parse(Parser *parser, ArenaAllocator *allocator) {
	ASTProgram *program = AST_createProgram(allocator);

	ASTFunction *function = NULL;

	while (!is_at_end(parser)) {
		function = parse_function(parser, allocator);
		ArrayList_add(ASTFunction, program->functions, *function);
		//AST_addFunctionToProgram(parser->allocator, program, function);
	}

	return program;
}
