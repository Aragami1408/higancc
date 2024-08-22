#include "ast.h"

#include <stdlib.h>

AST *AST_createNode(ArenaAllocator *a, ASTNodeType type) {
  AST *node = (AST*) ArenaAllocator_alloc(a, sizeof(AST));
  node->tag = type;
  return node;
}

AST *AST_createProgram(ArenaAllocator *a) {
  AST *node = AST_createNode(a, AST_PROGRAM);
  node->data.program.functions = NULL;
  return node;
}

void AST_addFunctionToProgram(ArenaAllocator *a, AST *program, AST *function) {
  // TODO(higanbana): considering const-correctness of 'function' param
  // as well as ASTList's node
  ASTList *new_ast = (ASTList *)ArenaAllocator_alloc(a, sizeof(ASTList));
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

AST *AST_createFunction(ArenaAllocator *a, const Token *name, AST *body) {
  AST *node = AST_createNode(a, AST_FUNCTION);
  node->data.function.name = *name;
  node->data.function.body = body;
  return node;
}

AST *AST_createReturn(ArenaAllocator *a, AST *exp) {
  AST *node = AST_createNode(a, AST_RETURN);
  node->data.ret.exp = exp;
  return node;
}

AST *AST_createExp(ArenaAllocator *a, const char *data) {
  AST *node = AST_createNode(a, AST_EXP);
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
