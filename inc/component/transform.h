#pragma once

#include "vector.h"

typedef struct {
	Vector* position;
	Vector* rotation;
	Vector* scale;
} Transform;

Transform* TRANSFORM_NewTransform(Vector* position);

void TRANSFORM_DestroyTransform(Transform* t);