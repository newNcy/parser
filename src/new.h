#pragma once
#include <malloc.h>
#include <string.h>

#ifndef NULL
#define NULL (void*)0
#endif

static void * _new(unsigned int size)
{
	static unsigned int sum = 0;
	void * ret = malloc(size);
	memset(ret, 0, size);
	if (ret) {
		sum += size;
		return ret;
	}
	return NULL;
}

#define new(X,...) new##X(__VA_ARGS__)
#define delete(X)	\
	if (X) {		\
		free((void*)X);	\
		X = 0;	\
	}

