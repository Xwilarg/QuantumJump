#include <stdlib.h>

#include "component/collider.h"
#include "object.h"
#include "rendering/render.h"

static void Update(Object* o, Game* game, Context* ctx, void* self)
{
	(void)game;
	(void)ctx;
	Collider* coll = (Collider*)self;

	RENDER_RenderCube(coll->min, coll->max, o->transform->rotation);
}

static void Destroy(void* self)
{
	Collider* coll = (Collider*)self;
	free(coll);
}

Collider* COLLIDER_New(Renderer* r)
{
	Collider* coll = malloc(sizeof(Collider));
	if (coll == NULL) return NULL;

	Mesh* mesh = r->mesh;
	float xMin = (float)mesh->vertices[0], xMax = xMin;
	float yMin = (float)mesh->vertices[1], yMax = yMin;
	float zMin = (float)mesh->vertices[2], zMax = zMin;

	for (size_t i = 3; i < mesh->numVertices; i += 3)
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

	coll->parent = ACOMPONENT_New(coll, COMPONENT_COLLIDER, &Update, &Destroy);
	return coll;
}

bool Collider_Check(Object* o, Game* game)
{
	Collider* coll = OBJECT_GetComponent(o, COMPONENT_COLLIDER);
	if (coll == NULL)
	{
		return false;
	}
	for (Object** to = game->objects; *to != NULL; to++)
	{
		Collider* targetColl = OBJECT_GetComponent(*to, COMPONENT_COLLIDER);
		if (targetColl != NULL)
		{
			return ((coll->min.x >= targetColl->min.x && coll->min.x <= targetColl->max.x) || (coll->max.x >= targetColl->min.x && coll->max.x <= targetColl->max.x))
				&& ((coll->min.y >= targetColl->min.y && coll->min.y <= targetColl->max.y) || (coll->max.y >= targetColl->min.y && coll->max.y <= targetColl->max.y));
		}
	}
	return false;
}