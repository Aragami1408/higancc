#include "tacky.h"
#include "utils.h"

ARRAYLIST_IMPL(TackyProgram);
ARRAYLIST_IMPL(TackyFunction);
ARRAYLIST_IMPL(TackyInstruction);


static TackyVal make_temp(Tacky *tacky) {
	TackyVal tmp;
	tmp.type = TACKY_VAL_VARIABLE;
	tmp.data.identifier = format_string("tmp.%d", tacky->tmp_counter);
	tacky->tmp_counter += 1;
	return tmp;
}

Tacky *Tacky_create(ArenaAllocator *allocator) {
	Tacky *tacky = ArenaAllocator_alloc(allocator, sizeof(tacky));
	tacky->allocator = allocator;
	tacky->tmp_counter = 0;
	tacky->instructions = ArrayList_init(TackyInstruction, tacky->allocator);
	return tacky;
}

TackyProgram *emit_tacky(Tacky *tacky, AST *ptr) {
}