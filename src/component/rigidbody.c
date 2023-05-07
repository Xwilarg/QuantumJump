#include <stdlib.h>

#include "component/rigidbody.h"
#include "object.h"

static void Update(Object* o, Context* ctx, void* self)
{
	Rigidbody* rb = (Rigidbody*)self;

	//TODO: Clean code and implement arithmetic in vector.h

	rb->linearVelocity->x *= 1 - rb->linearDrag * ctx->time->deltaTime;
	rb->linearVelocity->y *= 1 - rb->linearDrag * ctx->time->deltaTime;
	rb->linearVelocity->z *= 1 - rb->linearDrag * ctx->time->deltaTime;

	rb->angularVelocity->x *= 1 - rb->angularDrag * ctx->time->deltaTime;
	rb->angularVelocity->y *= 1 - rb->angularDrag * ctx->time->deltaTime;
	rb->angularVelocity->z *= 1 - rb->angularDrag * ctx->time->deltaTime;

	o->transform->position->x += rb->linearVelocity->x * ctx->time->deltaTime;
	o->transform->position->y += rb->linearVelocity->y * ctx->time->deltaTime;
	o->transform->position->z += rb->linearVelocity->z * ctx->time->deltaTime;

	o->transform->rotation->x += rb->angularVelocity->x * ctx->time->deltaTime;
	o->transform->rotation->y += rb->angularVelocity->y * ctx->time->deltaTime;
	o->transform->rotation->z += rb->angularVelocity->z * ctx->time->deltaTime;
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

	rb->parent = ACOMPONENT_New(rb, COMPONENT_RIGIDBODY, &Update, &Destroy);
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