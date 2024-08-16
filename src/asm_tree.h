#ifndef HIGANCC_ASM_TREE_H
#define HIGANCC_ASM_TREE_H

#include "common.h"

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
  struct ASMNode* instruction;
  struct ASMInstList* next;
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

ASMNode* ASMNode_createNode(ASMNodeType type);
ASMNode* ASMNode_createProgram(ASMNode* function);
ASMNode* ASMNode_createFunction(const char* name);
void ASMNode_addInstruction(ASMNode* function, ASMNode* instruction);
ASMNode* ASMNode_createMov(ASMNode* src, ASMNode* dst);
ASMNode* ASMNode_createRet(void);
ASMNode* ASMNode_createImm(int value);
ASMNode* ASMNode_createRegister(RegisterType reg);

void print_asm_tree(ASMNode *node, int depth);

void ASMNode_free(ASMNode* node);
#endif
