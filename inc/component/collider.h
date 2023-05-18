#pragma once

#include <stdbool.h>

#include "component/renderer.h"
#include "utils/vector.h"
#include "game.h"

typedef struct {
	AComponent* parent;

	Vector min;
	Vector max;

	void (*onCollision)(Game*, const Object*);
} Collider;

Collider* COLLIDER_New(Renderer* r);
const Object *COLLIDER_Check(Object* o, Vector incrPos, Game* game);