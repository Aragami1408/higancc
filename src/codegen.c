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
  default:
    fprintf(stderr, "Unsupported AST node type\n");
    return NULL;
  }
}

void generate_asm_code(FILE *out_file, ASMNode *node) {
  if (node == NULL) return;
  switch (node->type) {
  case ASM_PROGRAM:
    generate_asm_code(out_file, node->data.program.function);
    break;
  case ASM_FUNCTION: {
#ifdef HIGANCC_PLATFORM_WINDOWS
    fprintf(out_file,"  .globl %s\n", node->data.function.name);
    fprintf(out_file,"%s:\n", node->data.function.name);
#else
    fprintf(out_file,"  .globl _%s\n", node->data.function.name);
    fprintf(out_file,"_%s:\n", node->data.function.name);
#endif
    ASMInstList *curr = node->data.function.instructions;
    while (curr != NULL) {
      generate_asm_code(out_file, curr->instruction);
      curr = curr->next;
    }
  } break;
  case ASM_MOV: {
    fprintf(out_file,"  movl  ");
    generate_asm_code(out_file, node->data.mov.src);
    fprintf(out_file,", ");
    generate_asm_code(out_file, node->data.mov.dst);
    fprintf(out_file,"\n");
  } break;
  case ASM_RET: {
    fprintf(out_file,"  ret\n");
  } break;
  case ASM_IMM: {
    fprintf(out_file,"$%d", node->data.imm.value);
  } break;
  case ASM_REGISTER: {
    switch (node->data.reg.reg) {
    case REG_EAX:
      fprintf(out_file,"%%eax");
      break;
    }
  } break;
  }

#ifdef HIGANCC_PLATFORM_LINUX
  printf("  .section .note.GNU-stack,"",@progbits\n");
#endif
}
