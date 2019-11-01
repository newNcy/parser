#include <stdio.h>
#include "parser.h"




void visit(Source * s, Node * root)
{
	if (!root) return;
    if(root->op < END) return;
	//printNode(s,root);
	//printf(" -> ");
	Node * p = root->child;
	while (p) {
		printNode(s, p);
		p = p->sbling;
	}
	//printf("\n\n");
	p = root->child;
	while (p) {
		visit(s, p);
		p = p->sbling;
	}
}


int main(int argc, char *argv[])
{
	Source * test = sourceFromFile("test.c");
	if (!test) return -1;
	Node * ast = translation_unit(test);

	//后续遍历
	visit(test, ast);
	return 0;
}
