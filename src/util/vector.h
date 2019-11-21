#pragma once

#include "new.h"

typedef struct
{
	void ** data;
	unsigned long size,capacity;
}Vector;

Vector * newVector();
void vectorPushBack(Vector * vector, void * v);
void * vectorAt(Vector * vector, int idx);
void vectorErase(Vector * vector, int idx);
void deleteVector(Vector ** vector);
