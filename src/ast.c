#include "ast.h"

#include <stdlib.h>

AST *AST_createNode(ASTNodeType type) {
  AST *node = (AST*) malloc(sizeof(AST));
  node->tag = type;
  return node;
}

AST *AST_createProgram(void) {
  AST *node = AST_createNode(AST_PROGRAM);
  node->data.program.functions = NULL;
  return node;
}

void AST_addFunctionToProgram(AST *program, AST *function) {
  // TODO(higanbana): considering const-correctness of 'function' param
  // as well as ASTList's node
  ASTList *new_ast = (ASTList *)malloc(sizeof(ASTList));
  new_ast->node = function;
  new_ast->next = NULL;

  if (program->data.program.functions == NULL) {
    program->data.program.functions = new_ast;
  }
  else {
    ASTList *last = program->data.program.functions;
    while (last->next != NULL) {
      last = last->next;
    }
    last->next = new_ast;
  }
}

AST *AST_createFunction(const Token *name, AST *body) {
  AST *node = AST_createNode(AST_FUNCTION);
  node->data.function.name = *name;
  node->data.function.body = body;
  return node;
}

AST *AST_createReturn(AST *exp) {
  AST *node = AST_createNode(AST_RETURN);
  node->data.ret.exp = exp;
  return node;
}

AST *AST_createExp(const char *data) {
  AST *node = AST_createNode(AST_EXP);
  node->data.exp.data = data;
  return node;
}

void AST_print(const AST *ptr, int depth) {
  AST ast = *ptr;

#define TAB for (int i = 0; i < depth; i++) printf(" ");

  for (int i = 0; i < depth; i++) {
    printf(" ");
  }

  switch (ast.tag) {
  case AST_PROGRAM: {
    struct AST_PROGRAM program = ast.data.program;
    printf("Program(\n");
    ASTList *curr = program.functions;
    while (curr != NULL) {
      AST_print(curr->node, depth+1);
      curr = curr->next;
    }
    TAB
    printf(")\n");
  } break;
  case AST_FUNCTION: {
    struct AST_FUNCTION function = ast.data.function;
    Token name = function.name;
    printf("Function(name=\"%.*s\",body=\n", name.length, name.start);
    AST_print(function.body, depth+1);
    TAB
    printf(")\n");
  } break;
  case AST_RETURN: {
    struct AST_RETURN ret = ast.data.ret;
    printf("Return(\n");
    AST_print(ret.exp, depth+1);
    TAB
    printf(")\n");
  } break;
  case AST_EXP: {
    struct AST_EXP exp = ast.data.exp;
    printf("Constant(%s", exp.data);
    printf(")\n");
  } break;
  }
}


void AST_free(AST *ast) {
  if (ast == NULL) return;

  switch (ast->tag) {
  case AST_PROGRAM: {
    ASTList *curr = ast->data.program.functions;
    while (curr != NULL) {
      ASTList *next = curr->next;
      AST_free(curr->node);
      free(curr);
      curr = next;
    }
  } break;
  case AST_FUNCTION:
    AST_free(ast->data.function.body);
    break;
  case AST_RETURN:
    AST_free(ast->data.ret.exp);
    break;
  case AST_EXP:
    break;
  }

  free(ast);
}
