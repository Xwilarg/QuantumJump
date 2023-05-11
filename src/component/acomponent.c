#pragma once

#include <stdlib.h>
#include "component/acomponent.h"

AComponent* ACOMPONENT_New(void* self, Component component, void (*update)(Object*, Game*, Context*, void*), void (*destroy)(void*))
{
	AComponent* ac = malloc(sizeof(AComponent));
	if (ac == NULL) return NULL;

	ac->self = self;
	ac->component = component;
	ac->Update = update;
	ac->Destroy = destroy;
	return ac;
}

void ACOMPONENT_Destroy(AComponent* ac)
{
	ac->Destroy(ac->self);
	free(ac);
}

void ACOMPONENT_Update(AComponent* ac, Object* o, Game* game, Context* ctx)
{
	ac->Update(o, game, ctx, ac->self);
}