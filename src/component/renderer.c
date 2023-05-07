#include "component/renderer.h"

static void Update(void)
{

}

Renderer* RENDERER_NewRenderer(const char* meshPath)
{
	Renderer* r = malloc(sizeof(Renderer));
	if (r == NULL) return NULL;

	r->mesh = MESH_Load(meshPath);

	r->parent = malloc(sizeof(AComponent));
	r->parent->Update = &Update;
	return r;
}

void RENDERER_DestroyRenderer(Renderer* r)
{
	free(r->parent);
	MESH_Free(r->mesh);
	free(r);
}