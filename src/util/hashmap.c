#include "hashmap.h"
#include <stdio.h>
#include <string.h>

int compare(long a, long b)
{
	char * stra = (char*)a;
	char * strb = (char*)b;
	printf("compare %s %s\n",stra,strb);
	return strcmp(stra,strb);
}

void printRBNode(RBNode * cur)
{
	printf("[%s|%d]",cur->color?"B":"R",cur->pair.second);
}

void printTree(RBTree * tree)
{
	if(!tree || !tree->root) return;

	int len = 1024;
	RBNode * buff[1024] = {0};
	int w = 0, r = 0;
	buff[w++%len] = tree->root;
	int last = r;
	while (w != r) {
		RBNode * cur = buff[r];
		if (cur && (cur->left ||cur->right)) {
			printRBNode(cur);

			if (cur->left) {
				printRBNode(cur->left);
				buff[w] = cur->left;
				if (last == r) {
					last = w;
				}
				w = (w+1)%len;
			}
			if (cur->right) {
				printRBNode(cur->right);
				buff[w] = cur->right;
				if (last == r) {
					last = w;
				}
				w = (w+1)%len;
			}

			printf("\n");
		}

		r = (r+1)%len;
	}

}


int main ()
{
	RBTree * tree = newRBTree(compare);
	insert(tree,(Key)"a",1);
	insert(tree,(Key)"c",3);
	insert(tree,(Key)"b",2);
	/*
	insert(tree,(Key)"d",4);
	insert(tree,(Key)"e",5);
	insert(tree,(Key)"f",6);
	insert(tree,(Key)"g",7);
	insert(tree,(Key)"i",8);
	insert(tree,(Key)"j",9);
	insert(tree,(Key)"k",10);
	insert(tree,(Key)"l",11);
	insert(tree,(Key)"m",12);
	insert(tree,(Key)"n",13);
	insert(tree,(Key)"o",14);
	insert(tree,(Key)"p",15);
	insert(tree,(Key)"q",16);
	insert(tree,(Key)"r",17);
	insert(tree,(Key)"s",18);
	insert(tree,(Key)"t",19);
	insert(tree,(Key)"u",20);
	insert(tree,(Key)"v",21);
	insert(tree,(Key)"w",22);
	insert(tree,(Key)"x",23);
	insert(tree,(Key)"y",24);
	insert(tree,(Key)"z",25);
	*/

	printTree(tree);
	Key key = (Key)"i";
	RBNode * ret = search(tree, key);
	if (ret) {
		printf("map[%s]=%d\n", key, ret->pair.second);
	}else {
		printf("%s 不存在\n",key);
	}

	delete(tree,"b");
	delete(tree,"i");
}
