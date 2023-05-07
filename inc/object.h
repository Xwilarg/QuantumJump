#pragma once

#include "component/transform.h"
#include "component/acomponent.h"
#include "mesh.h"

struct Object
{
	Transform* transform;

	AComponent** components;
};

Object* OBJECT_NewObject(char* meshPath);
void OBJECT_DestroyObject(Object* o);
void OBJECT_Update(Object *o);