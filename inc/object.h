#pragma once

#include "component/transform.h"
#include "component/acomponent.h"
#include "context/context.h"

struct Object
{
	Transform* transform;

	AComponent** components;
};

Object* OBJECT_New();
void OBJECT_Destroy(Object* o);
void OBJECT_Update(Object *o, Context* ctx);
void OBJECT_AddComponent(Object* o, AComponent* newAc);