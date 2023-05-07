#pragma once

#include <stdlib.h>
#include "component/acomponent.h"

AComponent* ACOMPONENT_New(void* self, void (*update)(Object*, Context*, void*), void (*destroy)(void*))
{
	AComponent* ac = malloc(sizeof(AComponent));
	if (ac == NULL) return NULL;

	ac->self = self;
	ac->Update = update;
	ac->Destroy = destroy;
	return ac;
}

void ACOMPONENT_Destroy(AComponent* ac)
{
	ac->Destroy(ac->self);
	free(ac);
}

void ACOMPONENT_Update(AComponent* ac, Object* o, Context* ctx)
{
	ac->Update(o, ctx, ac->self);
}