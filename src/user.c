#include "component/renderer.h"
#include "component/rigidbody.h"
#include "component/collider.h"
#include "user.h"

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

// Constants
static const float _playerSpeed = 7500.f;

const Vector* GetCameraPosition(void)
{
	return &_cameraPos;
}

static void UpdateCameraPosition()
{
	_cameraPos = VECTOR_New(
		_camOffset.x + _player->transform->position.x,
		_camOffset.y + _player->transform->position.y,
		_camOffset.z + _player->transform->position.z
	);
}

void USER_Input(int key, bool isPressed)
{
	switch (key)
	{
	case 32: // Spacebar
		if (_playerRb->isOnGround)
		{
			RIGIDBODY_AddForce(_playerRb, VECTOR_New(.0f, 100.f, .0f));
		}
		break;

	case 87: // W
		_isUpPressed = isPressed;
		break;

	case 83: // S
		_isDownPressed = isPressed;
		break;

	case 65: // A
		_isLeftPressed = isPressed;
		break;

	case 68: // D
		_isRightPressed = isPressed;
		break;

	case 82: // R
		_player->transform->position = _initialPos;
		UpdateCameraPosition();
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
	), ctx->time->deltaTime * _playerSpeed);
	_playerRb->linearVelocity.x = dirVector.x;
	_playerRb->linearVelocity.z = dirVector.z;

	// Set camera position to follow player
	UpdateCameraPosition();
}

void USER_Init(Game* g, Context* ctx)
{
	(void)ctx;

	_cameraPos = VECTOR_New(0.f, 300.f, 10.f);
	_isLeftPressed = false;
	_isRightPressed = false;
	_isUpPressed = false;
	_isDownPressed = false;
	_canJump = true;

	{
		_player = OBJECT_New();
		_player->transform->position.z = -200.f; // TODO: Probably can do so we don't hardcode that uh

		Renderer* r = RENDERER_New("demo.mesh", "demo.tex");
		Collider* coll = COLLIDER_New(r);
		_playerRb = RIGIDBODY_New();

		OBJECT_AddComponent(_player, r->parent);
		OBJECT_AddComponent(_player, _playerRb->parent);
		OBJECT_AddComponent(_player, coll->parent);

		GAME_AddObject(g, _player);
	}

	{
		Object* ref = OBJECT_New();
		ref->transform->position.z = -200.f;
		ref->transform->position.x = -100.f;

		Renderer* r = RENDERER_New("demo.mesh", "demo.tex");
		Collider* coll = COLLIDER_New(r);
		Rigidbody* rb = RIGIDBODY_New();

		OBJECT_AddComponent(ref, r->parent);
		OBJECT_AddComponent(ref, rb->parent);
		OBJECT_AddComponent(ref, coll->parent);

		GAME_AddObject(g, ref);
	}

	_initialPos = _player->transform->position;
	_camOffset = VECTOR_New(
		_cameraPos.x - _player->transform->position.x,
		_cameraPos.y - _player->transform->position.y,
		_cameraPos.z - _player->transform->position.z
	);
}