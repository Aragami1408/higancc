#include "asm_tree.h"

#include <stdio.h>
#include <stdlib.h>


ASMNode* ASMNode_createNode(ASMNodeType type) {
  ASMNode* node = (ASMNode*)malloc(sizeof(ASMNode));
  node->type = type;
  return node;
}

ASMNode* ASMNode_createProgram(ASMNode* function) {
  ASMNode *node = ASMNode_createNode(ASM_PROGRAM);
  node->data.program.function = function; 
  return node;
}

ASMNode* ASMNode_createFunction(const char* name) {
  ASMNode *node = ASMNode_createNode(ASM_FUNCTION);
  node->data.function.name = name; 
  node->data.function.instructions = NULL;
  return node;
}

void ASMNode_addInstruction(ASMNode* function, ASMNode* instruction) {
  ASMInstList* new_inst = (ASMInstList*)malloc(sizeof(ASMInstList));
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

ASMNode* ASMNode_createMov(ASMNode* src, ASMNode* dst) {
  ASMNode *node = ASMNode_createNode(ASM_MOV);
  node->data.mov.src = src; 
  node->data.mov.dst = dst;
  return node;
}

ASMNode* ASMNode_createRet() {
  ASMNode *node = ASMNode_createNode(ASM_RET);
  return node;
}

ASMNode* ASMNode_createImm(int value) {
  ASMNode *node = ASMNode_createNode(ASM_IMM);
  node->data.imm.value = value;
  return node;
}

ASMNode* ASMNode_createRegister(RegisterType reg) {
  ASMNode *node = ASMNode_createNode(ASM_REGISTER);
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

void ASMNode_free(ASMNode* node) {
  if (node == NULL) return;

  switch (node->type) {
  case ASM_PROGRAM:
    ASMNode_free(node->data.program.function);
    break;
  case ASM_FUNCTION: {
    ASMInstList* curr = node->data.function.instructions;
    while (curr != NULL) {
      ASMInstList* next = curr->next;
      ASMNode_free(curr->instruction);
      free(curr);
      curr = next;
    }
  } break;
  case ASM_MOV:
    ASMNode_free(node->data.mov.src);
    ASMNode_free(node->data.mov.dst);
    break;
  case ASM_RET:
  case ASM_IMM:
  case ASM_REGISTER:
    break;
  }

  free(node);
}
