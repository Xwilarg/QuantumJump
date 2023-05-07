#include "object.h"
#include "component/renderer.h"
#include "component/rigidbody.h"

Object* OBJECT_New(char* meshPath)
{
	Object* o = malloc(sizeof(Object));
	if (o == NULL) return NULL;

	o->transform = TRANSFORM_New(VECTOR_New(0.f, 0.f, 0.f));

	Rigidbody* rb = RIGIDBODY_New();
	RIGIDBODY_AddTorque(rb, VECTOR_New(0.f, 10.f, 0.f));
	RIGIDBODY_AddForce(rb, VECTOR_New(-25.f, 0.f, 0.f));
	rb->angularDrag = .25f;
	o->components = malloc(sizeof(AComponent*) * 3);
	if (o->components == NULL) return NULL;
	o->components[0] = RENDERER_New(meshPath)->parent;
	o->components[1] = rb->parent;
	o->components[2] = NULL;
	return o;
}

void OBJECT_Destroy(Object* o)
{
	TRANSFORM_Destroy(o->transform);
	for (AComponent** ac = o->components; *ac != NULL; ac++)
	{
		ACOMPONENT_Destroy(*ac);
	}
	free(o->components);
	free(o);
}

void OBJECT_Update(Object* o, Context* ctx)
{
	for (AComponent** ac = o->components; *ac != NULL; ac++)
	{
		ACOMPONENT_Update(*ac, o, ctx);
	}
}