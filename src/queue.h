#pragma once

#include "lex.h"

struct QueueNode 
{
	long val;
	struct QueueNode * next;
};
typedef struct QueueNode QueueNode;

QueueNode * queueNew(long v);
void queuePush(QueueNode * q, long v);
long queueFront(QueueNode * q);


