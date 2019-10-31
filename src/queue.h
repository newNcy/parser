#pragma once

#include "lex.h"
struct QueueNode 
{
	long val;
	struct QueueNode * next;
};
typedef struct QueueNode QueueNode;

struct Queue
{
	int size;
	QueueNode * _root;
};
typedef struct Queue Queue;

Queue * queue();
void queuePush(Queue* q, long v);
long queueFront(Queue* q);


