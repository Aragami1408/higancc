#ifndef HIGANCC_CODEGEN_H
#define HIGANCC_CODEGEN_H

#include "ast.h"
#include "asm_tree.h"
#include "memory.h"


ASMNode *convert_ast_to_asm(ArenaAllocator *a, const AST *ast);

#endif
