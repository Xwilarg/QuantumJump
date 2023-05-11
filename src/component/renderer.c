#include "component/renderer.h"
#include "object.h"
#include "rendering/render.h"

static void Update(Object* o, Game* game, Context* ctx, void* self)
{
	(void)game;
	(void)ctx;
	Renderer* r = (Renderer*)self;

	RENDER_RenderMesh(r->mesh, o->transform);
}

static void Destroy(void* self)
{
	Renderer* r = (Renderer*)self;
	MESH_Free(r->mesh);
	free(r);
}

Renderer* RENDERER_New(const char* meshPath, const char* texturePath)
{
	Renderer* r = malloc(sizeof(Renderer));
	if (r == NULL) return NULL;

	r->mesh = MESH_Load(meshPath, texturePath);

	r->parent = ACOMPONENT_New(r, COMPONENT_RENDERER, &Update, &Destroy);
	return r;
}