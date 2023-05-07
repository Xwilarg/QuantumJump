#include "component/renderer.h"
#include "object.h"
#include "rendering/render.h"

static void Update(Object* o, Context* ctx, void* self)
{
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

Renderer* RENDERER_New(const char* meshPath)
{
	Renderer* r = malloc(sizeof(Renderer));
	if (r == NULL) return NULL;

	r->mesh = MESH_Load(meshPath);

	r->parent = ACOMPONENT_New(r, COMPONENT_RENDERER, &Update, &Destroy);
	return r;
}