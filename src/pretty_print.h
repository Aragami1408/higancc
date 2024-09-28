#ifndef HIGANCC_PRETTY_PRINT_H
#define HIGANCC_PRETTY_PRINT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ast.h"

void pretty_print_ast(const struct ASTProgram *program);

#endif
