#ifndef HIGANCC_CODEGEN_H
#define HIGANCC_CODEGEN_H

#include "ast.h"
#include "asm_tree.h"
#include "memory.h"

#include <stdio.h>
#include <stdlib.h>


ASMNode *convert_ast_to_asm(ArenaAllocator *a, const AST *ast);
void generate_asm_code(FILE *out_file, ASMNode *node);

#endif
