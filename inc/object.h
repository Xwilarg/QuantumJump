#pragma once

#include "component/transform.h"
#include "component/renderer.h"
#include "mesh.h"

typedef struct
{
	Transform* transform;

	Renderer* renderer;
} Object;

Object* OBJECT_NewObject(Mesh* mesh);
void OBJECT_DestroyObject(Object* o);