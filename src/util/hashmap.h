#pragma once
#include "../new.h"

typedef char bool;
#define true 1
#define false 0

enum Color 
{
	RED,//默认红色
	BLACK
};

enum Align
{
	LEFT = -1,
	NONE = 0,
	RIGHT = 1,
};
typedef enum Color Color;
typedef enum Align Align;



#define Key long
#define Value long 

struct Pair
{
	Key first;
	Value second;
};

typedef struct Pair Pair;
typedef int (*Compare)(Key, Key);
typedef Pair (*Save)(Key, Value);
typedef void (*Delete)(Pair);

struct RBNode
{
	Color color;
	struct RBNode * parent;
	struct RBNode * left;
	struct RBNode * right;
	Pair pair;
};
typedef struct RBNode RBNode;

struct RBTree
{
	RBNode * root;
	Compare compare;
	Save save;
	Delete delete;
};
typedef struct RBTree RBTree;

RBNode * newRBNode()
{
	RBNode * node = (RBNode*)new(sizeof(RBNode));
	return node;
}

Key ptrToKey(void *ptr)
{
	return (Key)ptr;
}
Value ptrToValue(void * ptr)
{
	return (Value)ptr;
}

void * keyToPtr(Key key)
{
	return (void*)key;
}

void * valueToPtr(Value value)
{
	return (void*)value;
}


bool checkFull(RBNode * node)
{
	if (!node || !node->left || !node->right) 
		return false;
	return true;
}

void reverseColor(RBNode * node) 
{
	if (!node) return;
	node->color ^= 1;
}

void swapColor(RBNode * a, RBNode * b)
{
	if (!a || !b) return;
	Color t = a->color;
	a->color = b->color;
	b->color = t;
}
void swapPair(RBNode *a, RBNode * b)
{
	if (!a || !b) return;
	Pair t = a->pair;
	a->pair = b->pair;
	b->pair = t;
}
void setChild(RBNode * parent, RBNode * child, Align pos)
{
	if (!parent || pos == NONE) return;

	if (child) 
		child->parent = parent;
	if (pos == LEFT) {
		parent->left = child;
	}else {
		parent->right = child;
	}
}

RBNode * getChild(RBNode * parent, Align pos)
{
	if (!parent || pos == NONE) return NULL;
	return pos == LEFT? parent->left : parent->right;
}


RBNode * rotate(RBNode * node, Align align) 
{
	if (!getChild(node, -align)) {
		return node;
	}

	RBNode * newRoot = getChild(node, -align);
	swapColor(node, newRoot);
	
	setChild(node, getChild(newRoot, align), -align);
	newRoot->parent = node->parent;
	setChild(newRoot, node, align);

	return newRoot;
}

RBNode * search(RBTree * tree,Key key) 
{
	if (!tree || !tree->root || !tree->compare) return NULL;

	RBNode * cur = tree->root;
	while (cur) {
		int res = tree->compare(cur->pair.first, key);
		if (!res) {
			return cur;
		}else if (res > 0) {
			cur = cur->left;
		}else {
			cur = cur->right;
		}
	}
	return NULL;
}


int nodePosition(RBNode * node)
{
	if (!node || !node->parent) return 0;

	if (node == node->parent->left) {
		return LEFT;
	}
	return RIGHT;
}

RBNode ** ptrNode(RBTree * tree, RBNode * node)
{
	if (!tree || !tree->root || !node) {
		return NULL;
	}
	if (node == tree->root) {
		return &tree->root;
	}

	return nodePosition(node) == LEFT? &node->parent->left : &node->parent->right;
}

void insert(RBTree * tree, Key key, Value value)
{
	if (!tree || !tree->compare) return;

	if (!tree->root) {
		RBNode * root = newRBNode();
		root->pair.first = key;
		root->pair.second = value;
		root->color = BLACK;	
		tree->root = root;
		return;
	}
	
	RBNode * parent = NULL;
	RBNode * cur = tree->root;
	Align posToInsert = NONE;

	while (cur) {
		parent = cur;
		int res = tree->compare(cur->pair.first, key);
		if (!res) {
			cur->pair.second = value;
			return;
		}else if (res > 0) {
			cur = cur->left;
			posToInsert = LEFT;
		}else {
			cur = cur->right;
			posToInsert = RIGHT;
		}
	}

	RBNode * me = newRBNode();
	me->pair.first = key;
	me->pair.second = value;

	if (parent) {
		setChild(parent, me, posToInsert);
	}

	while (parent->color == RED) {
		RBNode * grandParent = parent->parent;
		Align grandParentAlian = nodePosition(grandParent);
		Align parentAlian = nodePosition(parent);
		Align myAlign = nodePosition(me);

		RBNode * uncle = parentAlian == LEFT? grandParent->right : grandParent->left;

		if (uncle && uncle->color == RED) {
			uncle->color = parent->color = BLACK;
			grandParent->color = RED;
			
		}else { 
			RBNode * parentGrand = grandParent->parent;
			if (parentAlian != myAlign) {
				setChild(grandParent,rotate(parent, parentAlian), parentAlian);
			}

			if (!parentGrand) {
				tree->root = rotate(grandParent, -parentAlian);
			}else {
				setChild(parentGrand, rotate(grandParent, -parentAlian), grandParentAlian);
			}

		}
		me = grandParent;
		parent = me->parent;
		if (!parent) {
			if (me->color == RED) {
				me->color = BLACK;
			}
			break;
		}
	}

}

RBNode * postNode(RBTree * tree, RBNode * cur)
{
	if (!tree || !tree->root || !cur) return NULL;

	RBNode * post = NULL;
	if (cur->right) { //没有右节点
		post = cur->right;
		while (post->left) post = post->left;
	}else {
		post = cur->parent;
		while (post && nodePosition(post) == RIGHT) {
			post = post->parent;
		}
	}
	return post;
}

void delete(RBTree * tree, Key key)
{
	if (!tree || !tree->root || !tree->compare) return;

	RBNode * cur = tree->root;
	while (cur) {
		int res = tree->compare(cur->pair.first, key);
		if (!res) break;
		else if (res > 0) cur = cur->left;
		else cur = cur->right;
	}

	//找到删除节点
	if (!cur) {
		printf("not found\n");
		return;
	}

	//找后继节点
	RBNode * replace = postNode(tree, cur);
	if (replace)
		printf("post %ld\n",replace->pair.second);
	else printf("no post\n");

}


RBTree * newRBTree(Compare compare)
{
	RBTree * tree = (RBTree*)new(sizeof(RBTree));
	tree->compare = compare;
	return tree;
}




