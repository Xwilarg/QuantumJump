#include <stdlib.h>

#include "component/rigidbody.h"
#include "component/collider.h"
#include "object.h"

#include "config.h"

static void Update(Object* o, Game* game, Context* ctx, void* self)
{
	(void)game;
	Rigidbody* rb = (Rigidbody*)self;

	// Move rb with velocity
	rb->linearVelocity = VECTOR_Multiply(rb->linearVelocity, 1.f - rb->linearDrag * ctx->time->deltaTime);
	if (rb->useGravity)
	{
		rb->linearVelocity = VECTOR_Add(rb->linearVelocity, VECTOR_New(0.f, CONFIG_GRAVITY * ctx->time->deltaTime, 0.f));
	}
	rb->angularVelocity = VECTOR_Multiply(rb->angularVelocity, 1.f - rb->angularDrag * ctx->time->deltaTime);

	// Check collisions
	Vector targetPos = VECTOR_Multiply(rb->linearVelocity, ctx->time->deltaTime);
	rb->isOnGround = false;
	Object* collision = COLLIDER_Check(o, targetPos, game);

	if (o->transform->position.y + targetPos.y < .0f) // We are under the map
	{
		Collider* coll = OBJECT_GetComponent(o, COMPONENT_COLLIDER);
		if (coll->onCollision != NULL)
		{
			coll->onCollision(game, NULL);
		}
	}
	if (collision != NULL)
	{
		// Fire collision event
		Collider* coll = OBJECT_GetComponent(o, COMPONENT_COLLIDER);
		if (coll->onCollision != NULL)
		{
			coll->onCollision(game, collision);
		}

		// Check collisions on all axises
		Vector x = VECTOR_New(targetPos.x, .0f, .0f);
		Vector y = VECTOR_New(.0f, targetPos.y, .0f);
		Vector z = VECTOR_New(.0f, .0f, targetPos.z);
		if (COLLIDER_Check(o, y, game))
		{
			rb->linearVelocity.y = 0;
			if (targetPos.y < 0.f) // We are going down
			{
				rb->isOnGround = true;
			}
		}
		else
		{
			o->transform->position = VECTOR_Add(o->transform->position, y);
		}
		if (COLLIDER_Check(o, x, game))
		{
			rb->linearVelocity.x = 0;
		}
		else
		{
			o->transform->position = VECTOR_Add(o->transform->position, x);
		}
		if (COLLIDER_Check(o, z, game))
		{
			rb->linearVelocity.z = 0;
		}
		else
		{
			o->transform->position = VECTOR_Add(o->transform->position, z);
		}
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
	rb->isOnGround = false;

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