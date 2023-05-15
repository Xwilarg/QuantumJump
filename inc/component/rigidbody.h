#pragma once

#include <stdbool.h>

#include "component/acomponent.h"
#include "utils/vector.h"

typedef struct
{
	AComponent* parent;

	Vector linearVelocity;
	Vector angularVelocity;

	float linearDrag;
	float angularDrag;

	bool useGravity;

	bool isOnGround;
} Rigidbody;

Rigidbody* RIGIDBODY_New(void);

void RIGIDBODY_AddForce(Rigidbody* rb, Vector v);
void RIGIDBODY_AddTorque(Rigidbody* rb, Vector v);