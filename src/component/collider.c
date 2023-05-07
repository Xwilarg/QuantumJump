#include <stdlib.h>

#include "component/collider.h"
#include "component/renderer.h"
#include "object.h"

static void Update(Object* o, Context* ctx, void* self)
{
	Collider* coll = (Collider*)self;
}

static void Destroy(void* self)
{
	Collider* coll = (Collider*)self;
	free(rb);
}

Collider* COLLIDER_New(Renderer* r)
{
	Collider* coll = malloc(sizeof(Collider));
	if (coll == NULL) return NULL;

	Mesh* mesh = r->mesh;
	int xMin = (float)mesh->vertices[0], xMax = xMin;
	int yMin = (float)mesh->vertices[1], yMax = yMin;
	int zMin = (float)mesh->vertices[2], zMax = zMin;

	for (int i = 3; i < mesh->numVertices; i += 3)
	{
		float x = (float)mesh->vertices[i];
		float y = (float)mesh->vertices[i + 1];
		float z = (float)mesh->vertices[i + 2];

		if (x < xMin) xMin = x;
		if (x > xMax) xMax = x;
		if (y < yMin) yMin = y;
		if (y > yMax) yMax = y;
		if (z < zMin) zMin = z;
		if (z > zMax) zMax = z;
	}

	coll->min = VECTOR_New(xMin, yMin, zMin);
	coll->max = VECTOR_New(xMax, yMax, zMax);

	rb->parent = ACOMPONENT_New(rb, COMPONENT_COLLIDER, &Update, &Destroy);
	return rb;
}