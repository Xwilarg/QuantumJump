#pragma once

#include "context/context.h"

typedef struct Game Game;

typedef enum {
	COMPONENT_RIGIDBODY,
	COMPONENT_RENDERER,
	COMPONENT_COLLIDER
} Component;

typedef struct Object Object;

typedef struct {
	void (*Update)(Object*, Game*, Context*, void*);
	void (*Destroy)(void*);

	Object* object;
	void* self;
	Component component;
} AComponent;

AComponent* ACOMPONENT_New(void* self, Component component, void (*update)(Object*, Game*, Context*, void*), void (*destroy)(void*));
void ACOMPONENT_Destroy(AComponent* ac);
void ACOMPONENT_Update(AComponent* ac, Object* o, Game* game, Context* ctx);