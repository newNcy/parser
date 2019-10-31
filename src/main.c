#include <stdio.h>
#include "parser.h"



void printNode(Node * n)
{
	if (!n) return;
	if (n->op > END) {
		printf("%s ",nodeName[n->op - END - 1]);
    }else if (n->op < 255) {
        printf("%c ",n->op);
    }else {
		printf("%d ",n->op);
	}
	fflush(stdout);
}
void visit(Node * root)
{
	if (!root) return;
    if(root->op < END) return;
	printNode(root);
	printf(" -> ");
	Node * p = root->child;
	while (p) {
		printNode(p);
		p = p->sbling;
	}
	printf("\n\n");
	p = root->child;
	while (p) {
		visit(p);
		p = p->sbling;
	}
}

int main(int argc, char *argv[])
{
	Source * test = sourceFromFile("test.c");
	if (!test) return -1;
	Node * ast = translation_unit(test);

	//后续遍历
	visit(ast);
	return 0;
}
