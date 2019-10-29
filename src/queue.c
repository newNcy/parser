#include "queue.h"


QueueNode * queueNew(long v)
{
	QueueNode * n = (QueueNode*)new(sizeof(QueueNode));
	n->val = v;
	n->next = NULL;
	return n;
}

void queuePush(QueueNode * q, long v)
{
	if (!q || q->val) return; //val不为0不是头指针
	QueueNode * p = q;
	while (p->next != NULL) p = p->next;
	p->next = queueNew(v);
}

long queueFront(QueueNode *q)
{
	if (q->val || !q->next) return 0;
	return q->next->val;
}

long queuePop(QueueNode * q)
{
	if (q->val || !q->next ) return 0;
	QueueNode * f = q->next->next;
	long ret = f->val;
	free(q->next);
	q->next = f;
	return ret;
}

