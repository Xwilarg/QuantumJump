#pragma once

#include "context/context.h"

typedef enum {
	COMPONENT_Rigidbody,
	COMPONENT_Renderer
} Component;

typedef struct Object Object;

typedef struct {
	void (*Update)(Object*, Context*, void*);
	void (*Destroy)(void*);

	void* self;
	Component component;
} AComponent;

AComponent* ACOMPONENT_New(void* self, Component component, void (*update)(Object*, Context*, void*), void (*destroy)(void*));
void ACOMPONENT_Destroy(AComponent* ac);
void ACOMPONENT_Update(AComponent* ac, Object* o, Context* ctx);