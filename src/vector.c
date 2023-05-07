#include <stdlib.h>

#include "vector.h"

Vector* VECTOR_Zero()
{
	return VECTOR_New(0.f, 0.f, 0.f);
}

Vector* VECTOR_One()
{
	return VECTOR_New(1.f, 1.f, 1.f);
}

Vector* VECTOR_New(float x, float y, float z)
{
	Vector* v = malloc(sizeof(Vector));
	if (v == NULL) return NULL;

	v->x = x;
	v->y = y;
	v->z = z;
	return v;
}

void VECTOR_Destroy(Vector* v)
{
	free(v);
}