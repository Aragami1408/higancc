#include "higancc.h"

static Node *mul(void);
static Node *unary(void);
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
	Node *node = unary();
	
	for(;;) {
		if (consume('*'))
			node = new_node(ND_MUL, node, unary());
		else if (consume('/'))
			node = new_node(ND_DIV, node, unary());
		else
			return node;
	}
}

static Node *unary(void) {

	if (consume('+'))
		return primary();
	else if (consume('-'))
		return new_node(ND_SUB, new_node_num(0), primary());
	else
		return primary();
}

static Node *primary(void) {
	if (consume('(')) {
		Node *node = expr();
		expect(')');
		return node;
	}

	return new_node_num(expect_number());
}
