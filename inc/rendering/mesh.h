#pragma once

#include <stdbool.h>
#include <d3d9.h>

#include "texture.h"

typedef struct
{
	unsigned int numVertices;
	unsigned int numIndices;

	short* vertices;
	unsigned short* indices;
	float* uvs;

	Texture* texture;

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	LPDIRECT3DINDEXBUFFER9 indexBuffer;
} Mesh;

Mesh* MESH_Load(const char* meshPath, const char* texturePath);
void MESH_Free(Mesh* mesh);