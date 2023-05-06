#pragma once

#include <stdbool.h>

typedef struct
{
	unsigned int numVertices;
	unsigned int numIndices;

	short* vertices;
	unsigned short* indices;
	float* uvs;
} Mesh;

bool MESH_Load(const char* filename, Mesh* mesh);
void MESH_Free(Mesh* mesh);