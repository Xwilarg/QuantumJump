#pragma once

#include "component/acomponent.h"
#include "mesh.h"

typedef struct {
	AComponent* parent;

	Mesh* mesh;
} Renderer;

Renderer* RENDERER_New(const char* meshPath);