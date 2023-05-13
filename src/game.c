#include <stdlib.h>

#include "game.h"

Game* GAME_Init(void)
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
		OBJECT_Update(*o, g, ctx);
	}
}