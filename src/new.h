#pragma once
#include <malloc.h>
#include <string.h>

#ifndef NULL
#define NULL (void*)0
#endif

static void * new(unsigned int size)
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


