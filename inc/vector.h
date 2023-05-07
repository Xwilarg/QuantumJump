#pragma once

typedef struct
{
	float x, y, z;
} Vector;

Vector* VECTOR_Zero();
Vector* VECTOR_One();
Vector* VECTOR_NewVector(float x, float y, float z);
void VECTOR_DestroyVector(Vector* v);