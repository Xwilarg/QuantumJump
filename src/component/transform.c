#include <stdlib.h>
#include "component/transform.h"

Transform* TRANSFORM_NewTransform(Vector* position)
{
	Transform* t = malloc(sizeof(Transform));
	if (t == NULL) return NULL;

	t->position = position;
	t->rotation = VECTOR_Zero();
	t->scale = VECTOR_One();
	return t;
}

void TRANSFORM_DestroyTransform(Transform* t)
{
	VECTOR_DestroyVector(t->position);
	VECTOR_DestroyVector(t->rotation);
	VECTOR_DestroyVector(t->scale);
	free(t);
}