#pragma once

#include "component/acomponent.h"
#include "utils/vector.h"

typedef struct {
	AComponent* parent;

	Vector min;
	Vector max;
} Collider;

Collider* Collider_New(Renderer* r);