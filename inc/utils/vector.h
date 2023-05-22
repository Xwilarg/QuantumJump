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
float VECTOR_Length(Vector v);
Vector VECTOR_Magnitude(Vector v);
Vector VECTOR_Cross(Vector v1, Vector v2);
float VECTOR_Dot(Vector v1, Vector v2);