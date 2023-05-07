#pragma once

#include "component/transform.h"
#include "component/renderer.h"
#include "mesh.h"

typedef struct
{
	Transform* transform;

	Renderer* renderer;
} Object;

Object* OBJECT_NewObject(char* meshPath);
void OBJECT_DestroyObject(Object* o);
void OBJECT_Update(Object *o);