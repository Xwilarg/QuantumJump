#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "rendering/render.h"
#include "rendering/mesh.h"

#pragma pack(push, 1)
typedef struct
{
	uint16_t numVertices;
	uint16_t numIndices;
} MeshHeader;
#pragma pack(pop)

bool CreateBuffers(Mesh* mesh);

Mesh* MESH_Load(const char* filename)
{
	Mesh* mesh = malloc(sizeof(Mesh));
	if (mesh == NULL) return false;

	FILE* f;
	if (fopen_s(&f, filename, "rb") != 0)
	{
		assert(!"Failed to open mesh");
		return NULL;
	}

	// read the mesh header
	MeshHeader header;
	fread(&header, sizeof(header), 1, f);

	mesh->numVertices = header.numVertices;
	mesh->numIndices = header.numIndices;

	void* vertices = malloc(sizeof(short) * header.numVertices * 3);
	if (!vertices)
	{
		return NULL;
	}

	void* indices = malloc(sizeof(unsigned short) * header.numIndices);
	if (!indices)
	{
		free(vertices);
		return NULL;
	}

	void* uvs = malloc(sizeof(float) * header.numVertices * 2);
	if (!uvs)
	{
		free(vertices);
		free(indices);
		return NULL;
	}

	fread(vertices, sizeof(short), header.numVertices * 3, f);
	fread(uvs, sizeof(float), header.numVertices * 2, f);
	fread(indices, sizeof(unsigned short), header.numIndices, f);

	mesh->vertices = vertices;
	mesh->indices = indices;
	mesh->uvs = uvs;

	// create the render buffers
	if (!CreateBuffers(mesh))
	{
		return NULL;
	}

	fclose(f);

	return mesh;
}

static bool GetRenderVertex(Mesh* mesh, RenderVertex** buffer, int* size)
{
	// allocate the vertex buffer
	RenderVertex* vertices = malloc(sizeof(RenderVertex) * mesh->numVertices);

	if (!vertices)
	{
		assert(vertices);
		return false;
	}

	// convert to render vertex
	for (unsigned int i = 0; i < mesh->numVertices; i++)
	{
		vertices[i].x = mesh->vertices[i * 3];
		vertices[i].y = mesh->vertices[(i * 3) + 1];
		vertices[i].z = mesh->vertices[(i * 3) + 2];

		vertices[i].u = mesh->uvs[i * 2];
		vertices[i].v = mesh->uvs[(i * 2) + 1];
	}

	*buffer = vertices;
	*size = sizeof(RenderVertex) * mesh->numVertices;

	return true;
}

static bool CreateBuffers(Mesh* mesh)
{
	RenderVertex* vertices;
	int vertexBufferSize;

	int indexBufferSize = sizeof(short) * mesh->numIndices;

	if (!GetRenderVertex(mesh, &vertices, &vertexBufferSize))
	{
		return false;
	}

	if (FAILED(d3dDevice->lpVtbl->CreateVertexBuffer(d3dDevice, vertexBufferSize, NULL, RENDERFVF, D3DPOOL_DEFAULT, &mesh->vertexBuffer, NULL)))
	{
		return false;
	}

	void* buffer;

	// copy the vertices to the buffer
	mesh->vertexBuffer->lpVtbl->Lock(mesh->vertexBuffer, 0, 0, &buffer, 0);
	memcpy(buffer, vertices, vertexBufferSize);
	mesh->vertexBuffer->lpVtbl->Unlock(mesh->vertexBuffer);

	free(vertices);

	if (FAILED(d3dDevice->lpVtbl->CreateIndexBuffer(d3dDevice, indexBufferSize, NULL, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &mesh->indexBuffer, NULL)))
	{
		return false;
	}

	// copy the indices to the buffer
	mesh->indexBuffer->lpVtbl->Lock(mesh->indexBuffer, 0, 0, &buffer, 0);
	memcpy(buffer, mesh->indices, indexBufferSize);
	mesh->indexBuffer->lpVtbl->Unlock(mesh->indexBuffer);

	return true;
}

void MESH_Free(Mesh* mesh)
{
	free(mesh->indices);
	free(mesh->vertices);
	free(mesh->uvs);
}