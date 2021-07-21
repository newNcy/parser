#include <stdio.h>
#include "env.h"
#include "parser.h"


void visit(Source * s, Env * env, Node * root)
{
	static int tab = 0;
	if (!root) return;
    if(root->op < END) return;

	printf("[%02d]",tab);
	for (int i = 0 ; i < tab; i++) {
		printf(" ");
	}

	printNode(s, env, root);
	printf("-> ");
	Node * p = root->child;
	while (p) {
		printNode(s, env, p);
		p = p->sbling;
	}
	printf("\n");
	p = root->child;
	while (p) {
		tab += 1;
		visit(s, env, p);
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
	init_env(&env);
	if (!test) return -1;
	Node * ast = translation_unit(&env, test);

	//后续遍历
	visit(test, &env, ast);
	return 0;
}
