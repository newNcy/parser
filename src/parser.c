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
		p->child = c;
		return;
	}
	Node * n = p->child;
	while (n->sbling) {
		n = n->sbling;
	}
	n->sbling = c; 
}

void printTag(Tag t)
{
    if(t < 256) {
        printf("'%c'",t);
    }else {
        printf("[%d]",t);
    }
}

void match(Source * s,Tag t)
{
    Tag _t = look(s).tag;
    if (_t != t) {
        printf("expected ");
        printTag(t);
        printf("but get ");
        printTag(_t);
        printf("\n");
    }
	next(s);
}
/* 辅助过程 xx_*/


