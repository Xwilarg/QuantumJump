#pragma once

typedef struct Object Object;

typedef struct {
	void (*Update)(Object*, void*);
	void (*Destroy)(void*);

	void* self;
} AComponent;

AComponent* ACOMPONENT_New(void* self, void (*update)(Object*, void*), void (*destroy)(void*));
void ACOMPONENT_Destroy(AComponent* ac);
void ACOMPONENT_Update(AComponent* ac, Object* o);