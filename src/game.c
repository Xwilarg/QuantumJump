#include <stdlib.h>

#include "game.h"

#include "component/renderer.h"
#include "component/rigidbody.h"

// GAME SPECIFIC MODIFICTIONS GOES HERE

void GAME_USERInit(Game* g, Context* ctx)
{
	(void)ctx;

	g->Object = OBJECT_New();
	g->Object->transform->position.z = -200.f;

	Renderer* r = RENDERER_New("demo.mesh", "demo.tex");
	Rigidbody* rb = RIGIDBODY_New();
	RIGIDBODY_AddForce(rb, VECTOR_New(0.f, 25.f, 0.f));

	OBJECT_AddComponent(g->Object, r->parent);
	OBJECT_AddComponent(g->Object, rb->parent);
}

// END

Game* GAME_Init()
{
	Game* g = malloc(sizeof(Game));
	if (g == NULL) return NULL;

	g->Object = NULL;
	return g;
}

void GAME_Destroy(Game* g)
{
	if (g->Object != NULL)
	{
		OBJECT_Destroy(g->Object);
	}
	free(g);
}

void GAME_Update(Game* g, Context* ctx)
{
	if (g->Object != NULL)
	{
		OBJECT_Update(g->Object, ctx);
	}
}