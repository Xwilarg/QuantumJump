#include "mesh.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#pragma pack(push, 1)
typedef struct
{
	uint16_t numVertices;
	uint16_t numIndices;
} MeshHeader;
#pragma pack(pop)

bool MESH_Load(const char* filename, Mesh* mesh)
{
	FILE* f;
	if (fopen_s(&f, filename, "rb") != 0)
	{
		assert(!"Failed to open mesh");
		return false;
	}

	// read the mesh header
	MeshHeader header;
	fread(&header, sizeof(header), 1, f);

	mesh->numVertices = header.numVertices;
	mesh->numIndices = header.numIndices;

	void* vertices = malloc(sizeof(short) * header.numVertices * 3);
	if (!vertices)
	{
		return false;
	}

	void* indices = malloc(sizeof(unsigned short) * header.numIndices);
	if (!indices)
	{
		free(vertices);
		return false;
	}

	fread(vertices, sizeof(short), header.numVertices * 3, f);
	fread(indices, sizeof(unsigned short), header.numIndices, f);

	mesh->vertices = vertices;
	mesh->indices = indices;

	return true;
}

void MESH_Free(Mesh* mesh)
{
	free(mesh->indices);
	free(mesh->vertices);
}