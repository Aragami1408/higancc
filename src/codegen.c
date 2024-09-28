#include "codegen.h"


void generate_asm_code(FILE *out_file, ASMNode *node) {
	if (node == NULL) return;
	switch (node->type) {
		case ASM_PROGRAM:
			generate_asm_code(out_file, node->data.program.function);
			break;
		case ASM_FUNCTION: {
#ifdef HIGANCC_PLATFORM_WINDOWS
			fprintf(out_file,"  .globl %s\n", node->data.function.name);
			fprintf(out_file,"%s:\n", node->data.function.name);
#else
			fprintf(out_file,"  .globl _%s\n", node->data.function.name);
			fprintf(out_file,"_%s:\n", node->data.function.name);
#endif
			ASMInstList *curr = node->data.function.instructions;
			while (curr != NULL) {
				generate_asm_code(out_file, curr->instruction);
				curr = curr->next;
			}
		} break;
		case ASM_MOV: {
			fprintf(out_file,"  movl  ");
			generate_asm_code(out_file, node->data.mov.src);
			fprintf(out_file,", ");
			generate_asm_code(out_file, node->data.mov.dst);
			fprintf(out_file,"\n");
		} break;
		case ASM_RET: {
			fprintf(out_file,"  ret\n");
		} break;
		case ASM_IMM: {
			fprintf(out_file,"$%d", node->data.imm.value);
		} break;
		case ASM_REGISTER: {
			switch (node->data.reg.reg) {
				case REG_EAX:
					fprintf(out_file,"%%eax");
					break;
			}
		} break;
	}

#ifdef HIGANCC_PLATFORM_LINUX
	printf("  .section .note.GNU-stack,"",@progbits\n");
#endif
}
