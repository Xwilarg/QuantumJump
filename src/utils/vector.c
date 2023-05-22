#include <stdlib.h>
#include <math.h>

#include "utils/vector.h"

Vector VECTOR_New(float x, float y, float z)
{
	Vector v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

Vector VECTOR_Zero(void)
{
	return VECTOR_New(0.f, 0.f, 0.f);
}

Vector VECTOR_One(void)
{
	return VECTOR_New(1.f, 1.f, 1.f);
}

Vector VECTOR_Add(Vector v1, Vector v2)
{
	return VECTOR_New(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector VECTOR_Substract(Vector v1, Vector v2)
{
	return VECTOR_New(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector VECTOR_Multiply(Vector v1, float f)
{
	return VECTOR_New(v1.x * f, v1.y * f, v1.z * f);
}

Vector VECTOR_Divide(Vector v1, float f)
{
	return VECTOR_New(v1.x / f, v1.y / f, v1.z / f);
}

float VECTOR_Length(Vector v)
{
	return sqrt((double)v.x * v.x + (double)v.y * v.y + (double)v.z * v.z);
}

Vector VECTOR_Magnitude(Vector v)
{
	float length = VECTOR_Length(v);
	if (length == 0)
	{
		return VECTOR_Zero();
	}
	return VECTOR_Divide(v, length);
}