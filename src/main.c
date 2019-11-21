#include <stdio.h>
#include "lex.h"
#include "parser.h"
#include "util/vector.h"



void visit(Source * s, Node * root, int tab)
{
	if (!root) return;
	//printf("|");

	if (root > 10) {
		for (int i = 0; i < tab; i++) printf(" ");
		printNode(s,root);
		printf("\n");
	}
	if (root->child) {
		tab ++;
	}
	Node * p = root->child;
	while (p) {
		visit(s, p, tab);
		p = p->sbling;
	}
	if (root->child) {
		tab --;
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


void printVector(Vector * vector)
{
	if (!vector) return;
	for (int i = 0 ; i < vector->size; i ++) {
		printf("%d ",vectorAt(vector, i));
	}
}

int main(int argc, char *argv[])
{
	int _s[] = {};
	printf("%d\n", sizeof(_s));
	Source * test = sourceFromFile("test.c");
	Env * env = new(Env, NULL);
	if (!test) return -1;
	Node * ast = translation_unit(env, test);

	//后续遍历
	visit(test, ast,0);

	return 0;
}
