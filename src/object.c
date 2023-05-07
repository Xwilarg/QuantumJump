#include "object.h"

Object* OBJECT_NewObject(Mesh* mesh)
{
	Object* o = malloc(sizeof(Object));
	if (o == NULL) return NULL;

	o->transform = TRANSFORM_NewTransform(VECTOR_NewVector(0.f, 0.f, 0.f));
	o->mesh = mesh;
	return o;
}

void OBJECT_DestroyObject(Object* o)
{
	TRANSFORM_DestroyTransform(o->transform);
	free(o);
}