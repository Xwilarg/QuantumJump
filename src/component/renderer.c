#include "component/renderer.h"
#include "object.h"
#include "rendering/render.h"

static void Update(Object* o, Context* ctx, void* self)
{
	(void)ctx;
	Renderer* r = (Renderer*)self;

	// begin the frame
	RENDER_Clear();

	RENDER_RenderObject(r->mesh, o->transform);

	RENDER_Render();
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

	r->parent = ACOMPONENT_New(r, &Update, &Destroy);
	return r;
}