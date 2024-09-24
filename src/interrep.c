#include "interrep.h"


IR *IR_createNode(ArenaAllocator *a, IRNodeType type) {
  IR *node = (IR*)ArenaAllocator_alloc(a, sizeof(IR));
  node->type = type;
  return node;
}

IR *IR_createProgram(ArenaAllocator *a, IR *function) {
  IR *node = (IR*)IR_createNode(a, IR_PROGRAM);
  node->data.program.function = function;
  return node;
}

IR *IR_createFunction(ArenaAllocator *a, const char *identifier) {
  IR *node = IR_createNode(a, IR_FUNCTION);
  node->data.function.identifier = identifier;
  node->data.function.instructions = NULL;
  return node;
}

void IR_addInstruction(ArenaAllocator *a, IR *function, IR *instruction) {
  IRList *new_ir = (IRList *)ArenaAllocator_alloc(a, sizeof(IRList));
  new_ir->instruction = instruction;
  new_ir->next = NULL;

  if (function->data.function.instructions == NULL) {
    function->data.function.instructions = new_ir;
  }
  else {
    IRList *last = function->data.function.instructions;
    while (last->next != NULL) {
      last = last->next;
    }
    last->next = new_ir;
  }
}

IR *IR_createRet(ArenaAllocator *a, IR *val) {
  IR *node = IR_createNode(a, IR_RETURN);
  node->data.ret.val = val;
  return node;
}

IR *IR_createUnary(ArenaAllocator *a, IROperator op, IR *src, IR *dst) {
  IR *node = IR_createNode(a, IR_UNARY);
  node->data.unary.op = op;
  node->data.unary.src = src;
  node->data.unary.dst = dst;
  return node;
}

IR *IR_createConstant(ArenaAllocator *a, int val) {
  IR *node = IR_createNode(a, IR_CONSTANT);
  node->data.constant.val = val;
  return node;
}

IR *IR_createVar(ArenaAllocator *a, const char *identifier) {
  IR *node = IR_createNode(a, IR_VAR);
  node->data.var.identifier = identifier;
  return node;
}
