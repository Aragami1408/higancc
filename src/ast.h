#ifndef HIGANCC_AST_H
#define HIGANCC_AST_H

#include "token.h"
#include "memory.h"

typedef enum {
  AST_PROGRAM,
  AST_FUNCTION,
  AST_RETURN,
  AST_CONSTANT,
  AST_UNARY,
} ASTNodeType;

typedef enum {
  AST_UNARY_COMPLEMENT,
  AST_UNARY_NEGATE
} ASTOperator;

typedef struct ASTList ASTList;
typedef struct AST AST;

struct ASTList {
  AST *node;
  ASTList *next;
};

struct AST {

  ASTNodeType tag;

  union {
    struct AST_PROGRAM {
      ASTList *functions;
    } program;

    struct AST_FUNCTION {
      Token name;
      AST *body;
    } function;

    struct AST_RETURN {
      AST *exp;  
    } ret;

    struct AST_UNARY {
      ASTOperator op;
      AST *exp;
    } unary;

    struct AST_CONSTANT {
      int data;
    } constant;
  } data;
};

AST *AST_createNode(ArenaAllocator *a, ASTNodeType type);
AST *AST_createProgram(ArenaAllocator *a);
void AST_addFunctionToProgram(ArenaAllocator *a, AST *program, AST *function);
AST *AST_createFunction(ArenaAllocator *a, const Token *name, AST *body);
AST *AST_createReturn(ArenaAllocator *a, AST *exp);
AST *AST_createUnary(ArenaAllocator *a, ASTOperator op, AST *exp);
AST *AST_createConstant(ArenaAllocator *a, int data);

void AST_print(const AST *ptr, int depth);

#endif
