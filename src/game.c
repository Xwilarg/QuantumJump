#include <stdlib.h>

#include "game.h"

#include "component/renderer.h"
#include "component/rigidbody.h"

// GAME SPECIFIC MODIFICTIONS GOES HERE

void GAME_USERInit(Game* g, Context* ctx)
{
	(void)ctx;

	{
		Object* dynObj = OBJECT_New();
		dynObj->transform->position.z = -200.f;

		Renderer* r = RENDERER_New("demo.mesh", "demo.tex");
		Rigidbody* rb = RIGIDBODY_New();
		RIGIDBODY_AddForce(rb, VECTOR_New(0.f, 25.f, 0.f));

		OBJECT_AddComponent(dynObj, r->parent);
		OBJECT_AddComponent(dynObj, rb->parent);;

		GAME_AddObject(g, dynObj);
	}

	{
		Object* ref = OBJECT_New();
		ref->transform->position.z = -200.f;
		ref->transform->position.y = -70.f;

		Renderer* r = RENDERER_New("demo.mesh", "demo.tex");

		OBJECT_AddComponent(ref, r->parent);

		GAME_AddObject(g, ref);
	}
}

// END

Game* GAME_Init()
{
	Game* g = malloc(sizeof(Game));
	if (g == NULL) return NULL;

	g->objects = malloc(sizeof(Object*));
	if (g->objects == NULL)
		return NULL;

	g->objects[0] = NULL;
	return g;
}

void GAME_Destroy(Game* g)
{
	for (Object** o = g->objects; *o != NULL; o++)
	{
		OBJECT_Destroy(*o);
	}
	free(g);
}

void GAME_AddObject(Game* g, Object* obj)
{
	int size = 0;
	Object** o = g->objects;
	while (*(o++) != NULL)
	{
		size++;
	};
	g->objects = realloc(g->objects, sizeof(Object*) * (size + 2));
	if (g->objects == NULL)
		return; // TODO

	g->objects[size] = obj;
	g->objects[size + 1] = NULL;
}

void GAME_Update(Game* g, Context* ctx)
{
	for (Object** o = g->objects; *o != NULL; o++)
	{
		OBJECT_Update(*o, ctx);
	}
}