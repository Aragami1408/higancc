#ifndef HIGANCC_AST_H
#define HIGANCC_AST_H

#include "token.h"

typedef enum {
  AST_PROGRAM,
  AST_FUNCTION,
  AST_RETURN,
  AST_EXP,
} ASTNodeType;

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

    struct AST_EXP{
      const char *data;
    } exp;
  } data;
};

AST *AST_createNode(ASTNodeType type);
AST *AST_createProgram(void);
void AST_addFunctionToProgram(AST *program, AST *function);
AST *AST_createFunction(const Token *name, AST *body);
AST *AST_createReturn(AST *exp);
AST *AST_createExp(const char *data);

void AST_print(const AST *ptr, int depth);

void AST_free(AST *ptr);

#endif
