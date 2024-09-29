#ifndef HIGANCC_DUMP_H
#define HIGANCC_DUMP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ast.h"
#include "tacky.h"

void dump_tacky(const TackyProgram *program);
void dump_ast(const ASTProgram *program);

#endif
