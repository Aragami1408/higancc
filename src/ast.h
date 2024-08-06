#ifndef HIGANCC_AST_H
#define HIGANCC_AST_H

#include "token.h"

typedef struct AST AST;

typedef struct {
} ASTFunction;

struct AST {
  enum {
    AST_FUNCTION,
    AST_RETURN,
    AST_EXP,
  } tag;

  union {
    struct AST_FUNCTION {
      Token name;
      AST *body;
    } AST_FUNCTION;

    struct AST_RETURN {
      AST *exp;  
    } AST_RETURN;

    struct AST_EXP {
      const char *num;
    } AST_EXP;
  } data;
};

AST *ast_new(AST ast);
void ast_print(AST *ptr);
void ast_free(AST *ptr);

#define AST_NEW(tag, ...) \
  ast_new((AST){tag, {.tag = (struct tag){__VA_ARGS__}}})



#endif
