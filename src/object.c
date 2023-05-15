#include "object.h"
#include "component/renderer.h"
#include "component/rigidbody.h"

Object* OBJECT_New(void)
{
	Object* o = malloc(sizeof(Object));
	if (o == NULL) return NULL;

	o->transform = TRANSFORM_New(VECTOR_New(0.f, 0.f, 0.f));
	o->tag = (UserTag)(-1);

	o->components = malloc(sizeof(AComponent*));
	if (o->components == NULL) return NULL;
	o->components[0] = NULL;
	return o;
}

void OBJECT_AddComponent(Object* o, AComponent *newAc)
{
	int size = 0;
	AComponent** ac = o->components;
	while (*(ac++) != NULL)
	{
		size++;
	};
	o->components = realloc(o->components, sizeof(AComponent*) * (size + 2));
	if (o->components == NULL)
		return; // TODO

	o->components[size] = newAc;
	o->components[size + 1] = NULL;
}

void* OBJECT_GetComponent(Object* o, Component component)
{
	for (AComponent** ac = o->components; *ac != NULL; ac++)
	{
		if ((*ac)->component == component)
		{
			return (*ac)->self;
		}
	}
	return NULL;
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

void OBJECT_Update(Object* o, Game* game, Context* ctx)
{
	for (AComponent** ac = o->components; *ac != NULL; ac++)
	{
		ACOMPONENT_Update(*ac, o, game, ctx);
	}
}