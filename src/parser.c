#include "higancc.h"

static Node *mul(void);
static Node *primary(void);

static Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
	Node *node = (Node *)calloc(1, sizeof(Node));
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;

	return node;
}

static Node *new_node_num(int val) {
	Node *node = (Node *)calloc(1, sizeof(Node));
	node->kind = ND_NUM;
	node->val = val;

	return node;
}

Node *expr(void) {
	Node *node = mul();

	for (;;) {
		if (consume('+'))
			node = new_node(ND_ADD, node, mul());
		else if (consume('-'))
			node = new_node(ND_SUB, node, mul());
		else
			return node;
	}
}


static Node *mul(void) {
	Node *node = primary();
	
	for(;;) {
		if (consume('*'))
			node = new_node(ND_MUL, node, primary());
		else if (consume('/'))
			node = new_node(ND_DIV, node, primary());
		else
			return node;
	}
}

static Node *primary(void) {
	if (consume('(')) {
		Node *node = expr();
		expect(')');
		return node;
	}

	return new_node_num(expect_number());
}
