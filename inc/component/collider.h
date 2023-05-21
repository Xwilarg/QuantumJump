#pragma once

#include <stdbool.h>

#include "component/renderer.h"
#include "utils/vector.h"
#include "game.h"

typedef struct {
	AComponent* parent;

	Vector min;
	Vector max;

	bool triggerOnly;
	void (*onCollision)(Game*, Object*);
} Collider;

Collider* COLLIDER_New(Renderer* r);
bool COLLIDER_Check(Object* o, Vector incrPos, Game* game);