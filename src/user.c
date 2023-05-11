#include "component/renderer.h"
#include "component/rigidbody.h"
#include "user.h"

void USER_Init(Game* g, Context* ctx)
{
	(void)ctx;

	{
		Object* dynObj = OBJECT_New();
		dynObj->transform->position.z = -200.f;

		Renderer* r = RENDERER_New("demo.mesh", "demo.tex");
		Rigidbody* rb = RIGIDBODY_New();
		RIGIDBODY_AddForce(rb, VECTOR_New(0.f, 25.f, 0.f));

		OBJECT_AddComponent(dynObj, r->parent);
		OBJECT_AddComponent(dynObj, rb->parent);;

		GAME_AddObject(g, dynObj);
	}

	{
		Object* ref = OBJECT_New();
		ref->transform->position.z = -200.f;
		ref->transform->position.y = -70.f;

		Renderer* r = RENDERER_New("demo.mesh", "demo.tex");

		OBJECT_AddComponent(ref, r->parent);

		GAME_AddObject(g, ref);
	}
}