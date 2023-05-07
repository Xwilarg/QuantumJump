#pragma once

#include "vector.h"
#include "mesh.h"

typedef struct
{
	Vector position;
	Vector rotation;

	Mesh* mesh;
} Object;

void OBJECT_NewObject(Mesh* mesh, Object* object);