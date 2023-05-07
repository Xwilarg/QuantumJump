#include "component/rigidbody.h"
#include "object.h"

static void Update(Object* o, void* self)
{
	Rigidbody* rb = (Rigidbody*)self;

	o->transform->position->x += rb->linearVelocity->x;
	o->transform->position->y += rb->linearVelocity->y;
	o->transform->position->z += rb->linearVelocity->z;

	o->transform->rotation->x += rb->angularVelocity->x;
	o->transform->rotation->y += rb->angularVelocity->y;
	o->transform->rotation->z += rb->angularVelocity->z;
}

static void Destroy(void* self)
{
	Rigidbody* rb = (Rigidbody*)self;
	free(rb->linearVelocity);
	free(rb->angularVelocity);
	free(rb);
}

Rigidbody* RIGIDBODY_New()
{
	Rigidbody* rb = malloc(sizeof(Rigidbody));
	if (rb == NULL) return NULL;

	rb->linearVelocity = VECTOR_Zero();
	rb->angularVelocity = VECTOR_Zero();
	rb->linearDrag = 0.f;
	rb->angularDrag = 0.f;

	rb->parent = ACOMPONENT_New(rb, &Update, &Destroy);
	return rb;
}

void RIGIDBODY_AddForce(Rigidbody* rb, Vector* v)
{
	rb->linearVelocity->x += v->x;
	rb->linearVelocity->y += v->y;
	rb->linearVelocity->z += v->z;
}

void RIGIDBODY_AddTorque(Rigidbody* rb, Vector* v)
{
	rb->angularVelocity->x += v->x;
	rb->angularVelocity->y += v->y;
	rb->angularVelocity->z += v->z;
}