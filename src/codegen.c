#include "codegen.h"

ASMNode *convert_ast_to_asm(ArenaAllocator *a, const AST *ptr) {
  if (ptr == NULL) return NULL;
  AST ast = *ptr; 

  switch (ast.tag) {
  case AST_PROGRAM: {
    ASMNode *program = ASMNode_createProgram(a, convert_ast_to_asm(a, ast.data.program.functions->node));
    return program;
  }
  case AST_FUNCTION: {
    ASMNode *function = ASMNode_createNode(a, ASM_FUNCTION);
    Token function_name = ast.data.function.name;
    function->data.function.name = strndup(function_name.start, function_name.length);
    function->data.function.instructions = NULL;

    ASMNode *body_asm = convert_ast_to_asm(a, ast.data.function.body);
    if (body_asm != NULL && body_asm->type == ASM_FUNCTION) {
      ASMInstList *inst = body_asm->data.function.instructions; 
      while (inst != NULL) {
        ASMNode_addInstruction(a, function, inst->instruction);
        inst = inst->next;
      }
    }
    else if (body_asm != NULL) {
      ASMNode_addInstruction(a, function, body_asm);
    }

    return function;
  }
  case AST_RETURN: {
    ASMNode *src_node = convert_ast_to_asm(a, ast.data.ret.exp);
    ASMNode *dst_node = ASMNode_createRegister(a, REG_EAX);
    ASMNode *mov = ASMNode_createMov(a, src_node, dst_node);

    ASMNode *ret = ASMNode_createRet(a);

    ASMNode *dummy_func = ASMNode_createFunction(a, NULL);
    ASMNode_addInstruction(a, dummy_func, mov);
    ASMNode_addInstruction(a, dummy_func, ret);

    return dummy_func;
  }
  case AST_EXP: {
    ASMNode *imm = ASMNode_createImm(a, atoi(ast.data.exp.data));
    return imm;
  }
  default:
    fprintf(stderr, "Unsupported AST node type\n");
    return NULL;
  }
}
