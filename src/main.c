#include <stdio.h>
#include "parser.h"




void visit(Source * s, Node * root)
{
	static int tab = 0;
	if (!root) return;
    if(root->op < END) return;

	printf("[%d]",tab);
	for (int i = 0 ; i < tab; i++) {
		printf(" ");
	}

	printNode(s,root);
	printf("-> ");
	Node * p = root->child;
	while (p) {
		printNode(s, p);
		p = p->sbling;
	}
	printf("\n");
	p = root->child;
	while (p) {
		tab += 1;
		visit(s, p);
		p = p->sbling;
		tab -= 1;
	}
}


void exec(Source * s, Node * root)
{
	if (!s || !root || root->op < END) return;

	Node * p = root->child;
	while (p) {
		exec(s, p);
		p = p->sbling;
	}


}


int main(int argc, char *argv[])
{
	int _s[] = {};
	printf("%d\n", sizeof(_s));
	Source * test = sourceFromFile("test.c");
	Env env;
	if (!test) return -1;
	Node * ast = translation_unit(&env, test);

	//后续遍历
	visit(test, ast);
	return 0;
}
