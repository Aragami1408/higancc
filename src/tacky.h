#ifndef HIGANCC_TACKY_H
#define HIGANCC_TACKY_H

#include "common.h"
#include "memory.h"
#include "ast.h"
#include "arraylist.h"

typedef struct Program TackyProgram;
typedef struct Function TackyFunction;
typedef struct Instruction TackyInstruction;
typedef struct Val TackyVal;
typedef struct Tacky Tacky;

ARRAYLIST_PROTOTYPE(TackyProgram);
ARRAYLIST_PROTOTYPE(TackyFunction);
ARRAYLIST_PROTOTYPE(TackyInstruction);

typedef enum {
	TACKY_VAL_CONSTANT,
	TACKY_VAL_VARIABLE
} TackyValType;

typedef enum {
	TACKY_INST_RETURN,
	TACKY_INST_UNARY
} TackyInstructionType;

typedef enum {
	TACKY_OPERATOR_UNARY_COMPLEMENT,
	TACKY_OPERATOR_UNARY_NEGATE
} TackyOperator;

struct Val {
	TackyValType type;
	union {
		int constant;
		const char *identifier;
	} data;
};

struct Program {
	ArrayList(TackyFunction) *functions;
};

struct Function {
	const char *name;
	ArrayList(TackyInstruction) *body;
};

struct Instruction {
	TackyInstructionType type;
	union {
		TackyVal return_val;
		struct {
			TackyOperator op;
			TackyVal src;
			TackyVal dst;
		} unary;
	} data;
};

struct Tacky {
	ArrayList(TackyInstruction) *instructions;
	u32 tmp_counter;
	ArenaAllocator *allocator;
};

Tacky *Tacky_create(ArenaAllocator *allocator);
TackyProgram *emit_tacky(Tacky *tacky, AST *program);

#endif
