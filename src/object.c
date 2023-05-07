#include "object.h"

Object* OBJECT_NewObject(char* meshPath)
{
	Object* o = malloc(sizeof(Object));
	if (o == NULL) return NULL;

	o->transform = TRANSFORM_NewTransform(VECTOR_NewVector(0.f, 0.f, 0.f));
	o->renderer = RENDERER_NewRenderer(meshPath);
	return o;
}

void OBJECT_DestroyObject(Object* o)
{
	TRANSFORM_DestroyTransform(o->transform);
	RENDERER_DestroyRenderer(o->renderer);
	free(o);
}