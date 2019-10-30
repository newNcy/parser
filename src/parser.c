#include "parser.h"

Node * newNode(Op op)
{
	Node * node = (Node*)new(sizeof(Node));
	node->op = op;
	node->attr = -1;
	node->child = NULL;
	node->sbling = NULL;
	return node;
}
Node * newAttrNode(Token t)
{
	Node * n = newNode(t.tag);
	n->attr = t.attr;
	return n;
}
void addChild(Node * p, Node * c)
{
	if (p->child == NULL) {
		p->child = p;
	}
	Node * n = p->child;
	while (n->sbling) {
		n = n->sbling;
	}
	n->sbling = c; 
}

void match(Source * s,Tag t)
{
	if (look(s).tag != t) printf("需要%c\n",t);
	next(s);
}
/* 辅助过程 xx_*/


