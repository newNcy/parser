#include "parser.h"
#include "lex.h"

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
        printf("[%s]",tagName[t - Void]);
    }
}
void printNode(Source * s, Node * n)
{
	if (!n) return;
	if (n->op > END) {
		printf("%s ",nodeName[n->op - END - 1]);
    }else if (n->op < 255) {
        printf("%c ",n->op);
    }else {
		printTag(n->op);
		Tag _t = n->op;
		if (_t == Id || (ConstChar < _t && _t < ConstStr)) {
			printf(" (%s) ",s->stringPool.pool + n->attr);
		}else if (_t == ConstChar) {
			printf(" '%s' ",s->stringPool.pool + n->attr);
		}else if (_t == ConstStr) {
			printf(" \"%s\" ",s->stringPool.pool + n->attr);
		}
	}
	fflush(stdout);
}
int match(Source * s,Tag t)
{
    Tag _t = look(s).tag;
    if (_t != t) {
        printf("expected ");
        printTag(t);
        printf("but get ");
        printTag(_t);
        printf(":\n");
		println(s);
		return 0;
    }
	return 1;
}
/* 辅助过程 xx_*/


