#include <stdlib.h>

#include "component/rigidbody.h"
#include "component/collider.h"
#include "object.h"

static void Update(Object* o, Game* game, Context* ctx, void* self)
{
	(void)game;
	Rigidbody* rb = (Rigidbody*)self;

	rb->linearVelocity = VECTOR_Multiply(rb->linearVelocity, 1.f - rb->linearDrag * ctx->time->deltaTime);
	if (rb->useGravity)
	{
		rb->linearVelocity = VECTOR_Add(rb->linearVelocity, VECTOR_New(0.f, -100.f * ctx->time->deltaTime, 0.f));
	}
	rb->angularVelocity = VECTOR_Multiply(rb->angularVelocity, 1.f - rb->angularDrag * ctx->time->deltaTime);

	Vector targetPos = VECTOR_Multiply(rb->linearVelocity, ctx->time->deltaTime);
	if (COLLIDER_Check(o, targetPos, game) || o->transform->position.y + targetPos.y < .0f)
	{
		rb->linearVelocity = VECTOR_Zero();
	}
	else
	{
		o->transform->position = VECTOR_Add(o->transform->position, targetPos);
	}
	o->transform->rotation = VECTOR_Add(o->transform->rotation, VECTOR_Multiply(rb->angularVelocity, ctx->time->deltaTime));
}

static void Destroy(void* self)
{
	Rigidbody* rb = (Rigidbody*)self;
	free(rb);
}

Rigidbody* RIGIDBODY_New(void)
{
	Rigidbody* rb = malloc(sizeof(Rigidbody));
	if (rb == NULL) return NULL;

	rb->linearVelocity = VECTOR_Zero();
	rb->angularVelocity = VECTOR_Zero();
	rb->linearDrag = 0.f;
	rb->angularDrag = 0.f;

	rb->useGravity = true;

	rb->parent = ACOMPONENT_New(rb, COMPONENT_RIGIDBODY, &Update, &Destroy);
	return rb;
}

void RIGIDBODY_AddForce(Rigidbody* rb, Vector v)
{
	rb->linearVelocity = VECTOR_Add(rb->linearVelocity, v);
}

void RIGIDBODY_AddTorque(Rigidbody* rb, Vector v)
{
	rb->angularVelocity = VECTOR_Add(rb->angularVelocity, v);
}