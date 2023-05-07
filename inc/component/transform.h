#pragma once

#include "utils/vector.h"

typedef struct {
	Vector position;
	Vector rotation;
	Vector scale;
} Transform;

Transform* TRANSFORM_New(Vector position);

void TRANSFORM_Destroy(Transform* t);