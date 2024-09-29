#ifndef HIGANCC_TACKY_H
#define HIGANCC_TACKY_H

#include "common.h"
#include "memory.h"
#include "ast.h"
#include "arraylist.h"

typedef struct TackyProgram TackyProgram;
typedef struct TackyFunction TackyFunction;
typedef struct TackyInstruction TackyInstruction;
typedef struct TackyVal TackyVal;
typedef struct Tacky Tacky;

ARRAYLIST_PROTOTYPE(TackyProgram)
ARRAYLIST_PROTOTYPE(TackyFunction)
ARRAYLIST_PROTOTYPE(TackyInstruction)

typedef enum {
	TACKY_VAL_CONSTANT,
	TACKY_VAL_VARIABLE
} TackyValType;

typedef enum {
	TACKY_INSTRUCTION_RETURN,
	TACKY_INSTRUCTION_UNARY
} TackyInstructionType;

typedef enum {
	TACKY_OPERATOR_UNARY_COMPLEMENT,
	TACKY_OPERATOR_UNARY_NEGATE
} TackyOperator;

struct TackyVal {
	TackyValType type;
	union {
		int constant;
		const char *variable;
	};
};

struct TackyProgram {
	ArrayList(TackyFunction) *functions;
};

struct TackyFunction {
	const char *name;
	ArrayList(TackyInstruction) *body;
};

struct TackyInstruction {
	TackyInstructionType type;
	union {
		TackyVal return_val;
		struct {
			TackyOperator op;
			TackyVal src;
			TackyVal dst;
		} unary;
	};
};

struct Tacky {
	ArrayList(TackyInstruction) *instructions;
	u32 tmp_counter;
	ArenaAllocator *allocator;
};

Tacky *Tacky_create(ArenaAllocator *allocator);
TackyProgram *emit_tacky(Tacky *tacky, const ASTProgram *program);

#endif
