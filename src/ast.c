#include "ast.h"

#include <stdlib.h>

AST *ast_new(AST ast) {
  AST *ptr = (AST *)malloc(sizeof(AST));
  if (ptr) *ptr = ast;
  return ptr;
}

void ast_print(AST *ptr) {
  AST ast = *ptr;

  switch (ast.tag) {
  case AST_FUNCTION: {
    struct AST_FUNCTION data = ast.data.AST_FUNCTION;
    printf("Function(name=\"%.*s\",body=", data.name.length, data.name.start);
    ast_print(data.body);
    printf(")");
  } break;
  case AST_RETURN: {
    struct AST_RETURN data = ast.data.AST_RETURN;
    printf("Return(");
    ast_print(data.exp);
    printf(")");
  } break;

  case AST_EXP: {
    struct AST_EXP data = ast.data.AST_EXP;
    printf("Constant(%s", data.data);
    printf(")");
  } break;
  }
}

void ast_free(AST *ptr) {
  AST ast = *ptr;

  switch (ast.tag) {
  case AST_FUNCTION: {
    struct AST_FUNCTION data = ast.data.AST_FUNCTION;
    ast_free(data.body);
  } break;
  case AST_RETURN: {
    struct AST_RETURN data = ast.data.AST_RETURN;
    ast_free(data.exp);
  } break;

  case AST_EXP: {
    // struct AST_EXP data = ast.data.AST_EXP;
  } break;
  }
}
