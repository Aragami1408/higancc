#include "asm_tree.h"

#include <stdio.h>
#include <stdlib.h>


ASMNode* ASMNode_createNode(ArenaAllocator *a, ASMNodeType type) {
  ASMNode *node = (ASMNode*)ArenaAllocator_alloc(a, sizeof(ASMNode));
  node->type = type;
  return node;
}

ASMNode* ASMNode_createProgram(ArenaAllocator *a, ASMNode* function) {
  ASMNode *node = ASMNode_createNode(a, ASM_PROGRAM);
  node->data.program.function = function; 
  return node;
}

ASMNode* ASMNode_createFunction(ArenaAllocator *a, const char* name) {
  ASMNode *node = ASMNode_createNode(a, ASM_FUNCTION);
  node->data.function.name = name; 
  node->data.function.instructions = NULL;
  return node;
}

void ASMNode_addInstruction(ArenaAllocator *a, ASMNode* function, ASMNode* instruction) {
  ASMInstList *new_inst = (ASMInstList*)ArenaAllocator_alloc(a, sizeof(ASMInstList));
  new_inst->instruction = instruction;
  new_inst->next = NULL;

  if (function->data.function.instructions == NULL) {
    function->data.function.instructions = new_inst;
  } else {
    ASMInstList* last = function->data.function.instructions;
    while (last->next != NULL) {
      last = last->next;
    }
    last->next = new_inst;
  }
}

ASMNode* ASMNode_createMov(ArenaAllocator *a, ASMNode* src, ASMNode* dst) {
  ASMNode *node = ASMNode_createNode(a, ASM_MOV);
  node->data.mov.src = src; 
  node->data.mov.dst = dst;
  return node;
}

ASMNode* ASMNode_createRet(ArenaAllocator *a) {
  ASMNode *node = ASMNode_createNode(a, ASM_RET);
  return node;
}

ASMNode* ASMNode_createImm(ArenaAllocator *a, int value) {
  ASMNode *node = ASMNode_createNode(a, ASM_IMM);
  node->data.imm.value = value;
  return node;
}

ASMNode* ASMNode_createRegister(ArenaAllocator *a, RegisterType reg) {
  ASMNode *node = ASMNode_createNode(a, ASM_REGISTER);
  node->data.reg.reg = reg;
  return node;
}

void print_asm_tree(ASMNode *node, int depth) {
  if (node == NULL) return;

  for (int i = 0; i < depth; i++) {
    printf(" ");
  }

  switch (node->type) {
  case ASM_PROGRAM:
    printf("Program\n");
    print_asm_tree(node->data.program.function, depth+1);
    break;
  case ASM_FUNCTION: {
    printf("Function: %s\n", node->data.function.name);
    ASMInstList *curr = node->data.function.instructions;
    while (curr != NULL) {
      print_asm_tree(curr->instruction, depth+1);
      curr = curr->next;
    }
  } break;
  case ASM_MOV:
    printf("MOV\n");
    print_asm_tree(node->data.mov.src, depth+1);
    print_asm_tree(node->data.mov.dst, depth+1);
    break;
  case ASM_RET:
    printf("RET\n");
    break;
  case ASM_IMM:
    printf("Immediate: %d\n", node->data.imm.value);
    break;
  case ASM_REGISTER:
    printf("Register: ");
    switch (node->data.reg.reg) {
    case REG_EAX:
      printf("EAX\n");
      break;
    }
    break;
  default:
      printf("Unknown node type\n");
  }
}
