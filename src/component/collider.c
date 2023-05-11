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

bool COLLIDER_Check(Object* o, Vector incrPos, Game* game)
{
	Collider* coll = OBJECT_GetComponent(o, COMPONENT_COLLIDER);
	if (coll == NULL)
	{
		return false;
	}
	Vector offT = VECTOR_Add(o->transform->position, incrPos);
	for (Object** to = game->objects; *to != NULL; to++)
	{
		if (*to == o)
		{
			continue;
		}
		Collider* targetColl = OBJECT_GetComponent(*to, COMPONENT_COLLIDER);
		if (targetColl != NULL)
		{
			if (((offT.x + coll->min.x >= (*to)->transform->position.x + targetColl->min.x && offT.x + coll->min.x <= (*to)->transform->position.x + targetColl->max.x)
				|| (offT.x + coll->max.x >= (*to)->transform->position.x + targetColl->min.x && offT.x + coll->max.x <= (*to)->transform->position.x + targetColl->max.x))
				&&
				((offT.y + coll->min.y >= (*to)->transform->position.y + targetColl->min.y && offT.y + coll->min.y <= (*to)->transform->position.y + targetColl->max.y)
					|| (offT.y + coll->max.y >= (*to)->transform->position.y + targetColl->min.y && offT.y + coll->max.y <= (*to)->transform->position.y + targetColl->max.y)))
			{
				return true;
			}
		}
	}
	return false;
}