#ifndef HIGANCC_ASM_TREE_H
#define HIGANCC_ASM_TREE_H

#include "common.h"
#include "memory.h"

typedef enum {
  ASM_PROGRAM,
  ASM_FUNCTION,
  ASM_MOV,
  ASM_RET,
  ASM_IMM,
  ASM_REGISTER
} ASMNodeType;

typedef enum {
  REG_EAX,
} RegisterType;

typedef struct ASMNode ASMNode;
typedef struct ASMInstList ASMInstList;

struct ASMInstList {
  ASMNode* instruction;
  ASMInstList* next;
};

struct ASMNode {
  ASMNodeType type;
  union {
    struct {
      ASMNode* function;
    } program;
    struct {
      const char* name;
      ASMInstList* instructions;
    } function;
    struct {
      ASMNode* src;
      ASMNode* dst;
    } mov;
    struct {
      int value;
    } imm;
    struct {
      RegisterType reg;
    } reg;
  } data;
};

ASMNode* ASMNode_createNode(ArenaAllocator *a, ASMNodeType type);
ASMNode* ASMNode_createProgram(ArenaAllocator *a, ASMNode* function);
ASMNode* ASMNode_createFunction(ArenaAllocator *a, const char* name);
void ASMNode_addInstruction(ArenaAllocator *a, ASMNode* function, ASMNode* instruction);
ASMNode* ASMNode_createMov(ArenaAllocator *a, ASMNode* src, ASMNode* dst);
ASMNode* ASMNode_createRet(ArenaAllocator *a);
ASMNode* ASMNode_createImm(ArenaAllocator *a, int value);
ASMNode* ASMNode_createRegister(ArenaAllocator *a, RegisterType reg);

void print_asm_tree(ASMNode *node, int depth);

#endif
