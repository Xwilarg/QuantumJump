#include <math.h>

#include "component/renderer.h"
#include "component/rigidbody.h"
#include "component/collider.h"
#include "audio/audio.h"
#include "user.h"

#include "config.h"

// Key info
static bool _isUpPressed, _isDownPressed, _isLeftPressed, _isRightPressed;

// Player info
static Object* _player;
static Rigidbody* _playerRb;
static bool _canJump;
static Vector _initialPos;

// Camera info
static Vector _cameraPos;
static Vector _camOffset;

const Vector* GetCameraPosition(void)
{
	return &_cameraPos;
}

static void UpdateCameraPosition(void)
{
	_cameraPos = VECTOR_New(
		_camOffset.x + _player->transform->position.x,
		_camOffset.y + _player->transform->position.y,
		_camOffset.z + _player->transform->position.z
	);
}

static void ResetPlayer(void)
{
	_player->transform->position = _initialPos;
	UpdateCameraPosition();
}

static void OnPlayerCollision(Game* game, Object* collision)
{
	// If collision is NULL, it means we falled from a platform
	if (collision == NULL || collision->tag == USERTAG_TRAP)
	{
		ResetPlayer();
	}
	else if (collision->tag == USERTAG_OBJECTIVE)
	{
		GAME_RemoveObject(game, collision);
	}
}

void USER_Input(int key, bool isPressed)
{
	switch (key)
	{
	case 32: // Spacebar
		if (_playerRb->isOnGround)
		{
			RIGIDBODY_AddForce(_playerRb, VECTOR_New(.0f, CONFIG_JUMP_FORCE, .0f));
		}
		break;

	case 87: case 38: // W
		_isUpPressed = isPressed;
		break;

	case 83: case 40: // S
		_isDownPressed = isPressed;
		break;

	case 65: case 37: // A
		_isLeftPressed = isPressed;
		break;

	case 68: case 39: // D
		_isRightPressed = isPressed;
		break;

	case 82: // R
		ResetPlayer();
		break;
	}
}

void USER_Update(Game* g, Context* ctx)
{
	(void)g;

	// Move player
	Vector dirVector = VECTOR_Multiply(VECTOR_New(
		(_isLeftPressed ? 1.f : 0.f) + (_isRightPressed ? -1.f : 0.f),
		.0f,
		(_isUpPressed ? -1.f : 0.f) + (_isDownPressed ? 1.f : 0.f)
	), ctx->time->deltaTime * CONFIG_SPEED);
	_playerRb->linearVelocity.x = dirVector.x;
	_playerRb->linearVelocity.z = dirVector.z;

	if (dirVector.x != 0.f || dirVector.z != 0.f)
	{
		_player->transform->rotation = VECTOR_New(.0f, (float)atan2(dirVector.x, dirVector.z), .0f);
	}

	// Set camera position to follow player
	UpdateCameraPosition();
}

static void AddPlatform(Game* game, float x, float y, float z)
{
	Object* obj = OBJECT_New();
	obj->transform->position.z = -200.f + z;
	obj->transform->position.y = y;
	obj->transform->position.x = x;

	Renderer* r = RENDERER_New("res/plane.mesh", "demo.tex");
	Collider* coll = COLLIDER_New(r);

	OBJECT_AddComponent(obj, r->parent);
	OBJECT_AddComponent(obj, coll->parent);

	GAME_AddObject(game, obj);
}

static void AddObjective(Game* game, float x, float y, float z)
{
	Object* obj = OBJECT_New();
	obj->transform->position.z = -200.f + z;
	obj->transform->position.y = y;
	obj->transform->position.x = x;

	obj->tag = USERTAG_OBJECTIVE;

	Renderer* r = RENDERER_New("demo.mesh", "demo.tex");
	Collider* coll = COLLIDER_New(r);

	OBJECT_AddComponent(obj, r->parent);
	OBJECT_AddComponent(obj, coll->parent);

	GAME_AddObject(game, obj);
}

static void AddTrap(Game* game, float x, float z)
{
	Object* obj = OBJECT_New();
	obj->transform->position.z = -200.f + z;
	obj->transform->position.x = x;

	obj->tag = USERTAG_TRAP;

	Renderer* r = RENDERER_New("demo.mesh", "demo.tex");
	Collider* coll = COLLIDER_New(r);

	OBJECT_AddComponent(obj, r->parent);
	OBJECT_AddComponent(obj, coll->parent);

	GAME_AddObject(game, obj);
}

void USER_Init(Game* g, Context* ctx)
{
	(void)ctx;

	_cameraPos = VECTOR_New(0.f, 400.f, 200.f);
	_isLeftPressed = false;
	_isRightPressed = false;
	_isUpPressed = false;
	_isDownPressed = false;
	_canJump = true;

	AUDIO_Play(AUDIO_Load("res/BGM.sound", true));

	{
		_player = OBJECT_New();
		_player->transform->position.z = -200.f; // TODO: Probably can do so we don't hardcode that uh
		_player->transform->position.y = 100.f;

		Renderer* r = RENDERER_New("demo.mesh", "demo.tex");
		Collider* coll = COLLIDER_New(r);
		_playerRb = RIGIDBODY_New();

		coll->onCollision = OnPlayerCollision;

		OBJECT_AddComponent(_player, r->parent);
		OBJECT_AddComponent(_player, _playerRb->parent);
		OBJECT_AddComponent(_player, coll->parent);

		GAME_AddObject(g, _player);
	}
	AddTrap(g, -300.f, 0.f);
	AddPlatform(g, 0.f, 50.f, 0.f);

	AddPlatform(g, 400.f, 50.f, 0.f);
	AddObjective(g, 400.f, 100.f, 0.f);

	_initialPos = _player->transform->position;
	_camOffset = VECTOR_New(
		_cameraPos.x - _player->transform->position.x,
		_cameraPos.y - _player->transform->position.y,
		_cameraPos.z - _player->transform->position.z
	);
}