#ifndef HIGANCC_DUMP_H
#define HIGANCC_DUMP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ast.h"
#include "tacky.h"
#include "token.h"

void dump_tacky(const TackyProgram *program);
void dump_ast(const ASTProgram *program);
void dump_tokens(const ArrayList(Token) *tokens);

#endif
