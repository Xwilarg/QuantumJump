#pragma once

#include <stdlib.h>
#include "component/acomponent.h"

AComponent* ACOMPONENT_NewAComponent(void* self, void (*update)(void), void (*destroy)(void*))
{
	AComponent* ac = malloc(sizeof(AComponent));
	if (ac == NULL) return NULL;

	ac->self = self;
	ac->Update = update;
	ac->Destroy = destroy;
	return ac;
}

void ACOMPONENT_DestroyComponent(AComponent* ac)
{
	ac->Destroy(ac->self);
	free(ac);
}

void ACOMPONENT_Update(AComponent* ac, Object* o)
{
	ac->Update(o, ac->self);
}