#include "object.h"
#include "component/renderer.h"

Object* OBJECT_New(char* meshPath)
{
	Object* o = malloc(sizeof(Object));
	if (o == NULL) return NULL;

	o->transform = TRANSFORM_New(VECTOR_New(0.f, 0.f, 0.f));

	o->components = malloc(sizeof(AComponent*) * 2);
	if (o->components == NULL) return NULL;
	o->components[0] = RENDERER_New(meshPath)->parent;
	o->components[1] = NULL;
	return o;
}

void OBJECT_Destroy(Object* o)
{
	TRANSFORM_Destroy(o->transform);
	for (AComponent** ac = o->components; *ac != NULL; ac++)
	{
		ACOMPONENT_Destroy(*ac);
	}
	free(o);
}

void OBJECT_Update(Object* o)
{
	// rotate the object
	o->transform->rotation->y += 0.1f;

	for (AComponent** ac = o->components; *ac != NULL; ac++)
	{
		ACOMPONENT_Update(*ac, o);
	}
}