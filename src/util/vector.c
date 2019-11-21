#include "vector.h"
#include "map.h"


#include <string.h>
Vector * newVector()
{
	Vector * vector = (Vector*)_new(sizeof(vector));
	vector->capacity = 10;
	vector->data = (void**)_new(sizeof(char*)*vector->capacity);
	return vector;
}

void vectorPushBack(Vector * vector, void * v) 
{
	if (!vector) return;
	if (vector->size == vector->capacity) {
		unsigned long newCap = vector->capacity*2;
		void ** old = vector->data;
		vector->data = (void**)_new(sizeof(void*)*newCap);
		memcpy(vector->data, old, sizeof(void*)*vector->capacity);
		vector->capacity = newCap;
	}
	vector->data[vector->size++] = v;
}
void * vectorAt(Vector * vector, int idx)
{
	if (!vector) return NULL;
	if (idx < 0 || idx >= vector->size) return NULL;
	return vector->data[idx];
}
void vectorErase(Vector * vector, int idx)
{
	if (!vector) return;
	if (idx < 0 || idx >= vector->size) return;

	vector->size --;
	for (int i = idx; i < vector->size; i++) {
		vector->data[i] = vector->data[i+1];
	}
}

void deleteVector(Vector ** vector)
{
	if (!vector || !*vector) {
		delete((*vector)->data);
	}
}
