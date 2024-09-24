#ifndef HIGANCC_INTERREP_H
#define HIGANCC_INTERREP_H

#include "common.h"
#include "memory.h"
#include "ast.h"

typedef enum {
  IR_PROGRAM,
  IR_FUNCTION,
  IR_RETURN,
  IR_UNARY,
  IR_CONSTANT,
  IR_VAR
} IRNodeType;

typedef enum {
  IR_OPERATOR_UNARY_COMPLEMENT,
  IR_OPERATOR_UNARY_NEGATE
} IROperator;

typedef struct IRList IRList;
typedef struct IR IR;

struct IRList {
  IR *instruction;
  IRList *next;
};

struct IR {
  IRNodeType type;

  union {
    struct {
      IR *function;
    } program;

    struct {
      const char *identifier;
      IRList *instructions;
    } function;
    
    struct {
      IR *val;
    } ret;

    struct {
      IROperator op;
      IR *src;
      IR *dst;
    } unary;

    struct {
      int val;
    } constant;

    struct {
      const char *identifier;
    } var;
  } data;
};

IR *IR_createNode(ArenaAllocator *a, IRNodeType type);
IR *IR_createProgram(ArenaAllocator *a, IR *function);
IR *IR_createFunction(ArenaAllocator *a, const char *identifier);
void IR_addInstruction(ArenaAllocator *a, IR *function, IR *instruction);
IR *IR_createRet(ArenaAllocator *a, IR *val);
IR *IR_createUnary(ArenaAllocator *a, IROperator op, IR *src, IR *dst);
IR *IR_createConstant(ArenaAllocator *a, int val);
IR *IR_createVar(ArenaAllocator *a, const char *identifier);

IR *IR_emit(AST *ast);

#endif
