#pragma once

#include "component/transform.h"
#include "mesh.h"

typedef struct
{
	Transform* transform;

	Mesh* mesh;
} Object;

Object* OBJECT_NewObject(Mesh* mesh);
void OBJECT_DestroyObject(Object* o);