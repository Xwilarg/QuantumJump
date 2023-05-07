#include <stdlib.h>
#include "component/transform.h"

Transform* TRANSFORM_New(Vector* position)
{
	Transform* t = malloc(sizeof(Transform));
	if (t == NULL) return NULL;

	t->position = position;
	t->rotation = VECTOR_Zero();
	t->scale = VECTOR_One();
	return t;
}

void TRANSFORM_Destroy(Transform* t)
{
	VECTOR_Destroy(t->position);
	VECTOR_Destroy(t->rotation);
	VECTOR_Destroy(t->scale);
	free(t);
}