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
/* 使用的自定义比较，存储，删除函数,第一个参数是持有者 */
typedef int (*Compare)(void *, Key, Key);
typedef void (*Save)(void *, Pair *, Key, Value);
typedef void (*Delete)(void *,Pair *);

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
	void * onwer;
	Compare compare;
	Save save;
	Delete delete;
};
typedef struct RBTree RBTree;

RBTree * newRBTree(void *, Compare,Save,Delete);
void insert(RBTree * tree, Key key, Value value);
void delete(RBTree * tree, Key key);
void deleteRBTree(RBTree * tree);

