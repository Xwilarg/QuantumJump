#pragma once

#include "component/transform.h"
#include "component/acomponent.h"
#include "context/context.h"

typedef enum UserTag UserTag;

struct Object
{
	Transform* transform;

	AComponent** components;

	UserTag tag;
};

Object* OBJECT_New(void);
void OBJECT_Destroy(Object* o);
void OBJECT_Update(Object *o, Game* game, Context* ctx);
void OBJECT_AddComponent(Object* o, AComponent* newAc);
void* OBJECT_GetComponent(Object* o, Component component);