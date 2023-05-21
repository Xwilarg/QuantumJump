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

	for (size_t i = 0; i < mesh->numVertices; i ++)
	{
		float x = (float)mesh->vertices[i * 3];
		float y = (float)mesh->vertices[i * 3 + 1];
		float z = (float)mesh->vertices[i * 3 + 2];

		if (x < xMin) xMin = x;
		if (x > xMax) xMax = x;
		if (y < yMin) yMin = y;
		if (y > yMax) yMax = y;
		if (z < zMin) zMin = z;
		if (z > zMax) zMax = z;
	}

	if (yMin == yMax)
	{
		yMin -= 1.f;
		yMax += 1.f;
	}

	coll->min = VECTOR_New(xMin, yMin, zMin);
	coll->max = VECTOR_New(xMax, yMax, zMax);
	coll->onCollision = NULL;

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
	Vector oMin = VECTOR_Add(offT, coll->min);
	Vector oMax = VECTOR_Add(offT, coll->max);

	bool didCollide = false;
	for (Object** to = game->objects; *to != NULL; to++)
	{
		if (*to == o)
		{
			continue;
		}
		Collider* targetColl = OBJECT_GetComponent(*to, COMPONENT_COLLIDER);
		if (targetColl != NULL)
		{
			Vector tarMin = VECTOR_Add((*to)->transform->position, targetColl->min);
			Vector tarMax = VECTOR_Add((*to)->transform->position, targetColl->max);
			bool xCheck = (oMin.x >= tarMin.x && oMin.x <= tarMax.x) || (oMax.x >= tarMin.x && oMax.x <= tarMax.x);
			bool yCheck = (oMin.y >= tarMin.y && oMin.y <= tarMax.y) || (oMax.y >= tarMin.y && oMax.y <= tarMax.y);
			bool zCheck = (oMin.z >= tarMin.z && oMin.z <= tarMax.z) || (oMax.z >= tarMin.z && oMax.z <= tarMax.z);
			if (xCheck && yCheck && zCheck)
			{
				if (coll->onCollision != NULL)
				{
					coll->onCollision(game, *to);
				}
				didCollide = true;
			}
		}
	}
	return didCollide;
}