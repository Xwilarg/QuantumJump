#pragma once

#include <stdbool.h>

#include "component/renderer.h"
#include "utils/vector.h"
#include "game.h"

typedef struct {
	AComponent* parent;

	Vector min;
	Vector max;
} Collider;

Collider* Collider_New(Renderer* r);
bool Collider_Check(Object* o, Game* game);