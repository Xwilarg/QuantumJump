#pragma once

typedef struct
{
	float x, y, z;
} Vector;

Vector VECTOR_Zero(void);
Vector VECTOR_One(void);
Vector VECTOR_New(float x, float y, float z);
Vector VECTOR_Add(Vector v1, Vector v2);
Vector VECTOR_Substract(Vector v1, Vector v2);
Vector VECTOR_Multiply(Vector v1, float f);
Vector VECTOR_Divide(Vector v1, float f);