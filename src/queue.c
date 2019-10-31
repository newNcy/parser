#include "queue.h"
#include "parser.h"


Queue * queue()
{
	return new(sizeof(Queue));
}
QueueNode * newQueueNode(long v)
{
	QueueNode * n = (QueueNode*)new(sizeof(QueueNode));
	n->val = v;
	return n;
}

void queuePush(Queue* q, long v)
{
	if (!q) return;

	QueueNode * n = newQueueNode(v);
	if (q->_root) {
		QueueNode * qn = q->_root;
		while (qn->next) {
			qn = qn->next;
		}
		
		qn->next = n;
	}else {
		q->_root = n;
	}

	q->size ++;
}

long queueFront(Queue*q)
{
	if (!q || !q->size) return 0;
	return  q->_root->val;
}

long queuePop(Queue* q)
{
	if (!q || !q->size) return 0;
	QueueNode * f = q->_root;
	long ret = f->val;
	free(q->_root);
	q->_root= f;
	return ret;
}

