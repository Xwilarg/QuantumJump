#pragma once

typedef struct Object Object;

typedef struct {
	void (*Update)(Object*, void*);
	void (*Destroy)(void*);

	void* self;
} AComponent;

AComponent* ACOMPONENT_NewAComponent(void* self, void (*update)(void), void (*destroy)(void*));
void ACOMPONENT_DestroyComponent(AComponent* ac);
void ACOMPONENT_Update(AComponent* ac, Object* o);