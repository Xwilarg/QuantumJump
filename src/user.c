#include <math.h>

#include "component/renderer.h"
#include "component/rigidbody.h"
#include "component/collider.h"
#include "audio/audio.h"
#include "user.h"
#include "rendering/render.h"

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

static int collectibleLeft;

static float _quantumEnergy;

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
	_playerRb->linearVelocity = VECTOR_Zero(); // Reset player velocity so it doesn't go through platform
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
		collectibleLeft--;
		if (collectibleLeft == 0)
		{
			// TODO: Victory
		}
	}
	else if (collision->tag == USERTAG_CHECKPOINT)
	{
		_initialPos = VECTOR_New(collision->transform->position.x, _initialPos.y, collision->transform->position.z);
	}
}

void USER_Input(int key, bool isPressed)
{
	switch (key)
	{
	case 32: // Spacebar
		if (_playerRb->isOnGround)
		{
			RIGIDBODY_AddForce(_playerRb, VECTOR_New(.0f, CONFIG_JUMP_FORCE * _quantumEnergy / 100.f, .0f));
			_quantumEnergy = 0;
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

static char* IntToString(int nb)
{
	int h = nb / 100;
	int d = nb / 10;
	int u = nb % 10;

	int size = 2;
	if (h > 0) size = 4;
	else if (d > 0) size = 3;

	char* str = malloc(sizeof(char) * size);
	if (str == NULL) return NULL;
	if (h > 0) str[0] = h + '0';
	if (h > 0 || d > 0) str[size - 3] = d + '0';
	str[size - 2] = u + '0';
	str[size - 1] = 0;

	return str;
}

void USER_Update(Game* g, Context* ctx)
{
	(void)g;

	// Move player
	Vector dirVector = VECTOR_Multiply(VECTOR_Magnitude(VECTOR_New(
		(_isLeftPressed ? 1.f : 0.f) + (_isRightPressed ? -1.f : 0.f),
		.0f,
		(_isUpPressed ? -1.f : 0.f) + (_isDownPressed ? 1.f : 0.f)
	)), ctx->time->deltaTime * CONFIG_SPEED);
	_playerRb->linearVelocity.x = dirVector.x;
	_playerRb->linearVelocity.z = dirVector.z;

	if (dirVector.x != 0.f || dirVector.z != 0.f)
	{
		_player->transform->rotation = VECTOR_New(.0f, (float)atan2(dirVector.x, dirVector.z), .0f);
	}

	if (_playerRb->isOnGround)
	{
		_quantumEnergy += ctx->time->deltaTime * CONFIg_ENERGY_RELOAD_RATE;
		if (_quantumEnergy > 100.f) _quantumEnergy = 100.f;

		char* nb = IntToString(_quantumEnergy);
		char* label = "Energy - ";
		size_t size = strlen(label) + strlen(nb) + 1;
		char* title = calloc(size, sizeof(char));
		strcat_s(title, size, label);
		strcat_s(title, size, nb);
		EditWindowTitle(title);
		free(nb);
	}

	// Set camera position to follow player
	UpdateCameraPosition();
}

static Object* AddObject(Game* game, int x, int y, int z, char* mesh, char* texture, bool triggerOnly)
{
	Object* obj = OBJECT_New();
	obj->transform->position = VECTOR_New(CONFIG_GRID_UNIT * x, CONFIG_GRID_UNIT_UP * y, CONFIG_GRID_UNIT * z - 200.f);

	Renderer* r = RENDERER_New(mesh, texture);
	Collider* coll = COLLIDER_New(r);

	coll->triggerOnly = triggerOnly;

	OBJECT_AddComponent(obj, r->parent);
	OBJECT_AddComponent(obj, coll->parent);

	GAME_AddObject(game, obj);

	return obj;
}

static void AddPlatform(Game* game, int x, int y, int z)
{
	AddObject(game, x, y, z, "res/plane.mesh", "res/blue.tex", false);
}

static void AddObjective(Game* game, int x, int y, int z)
{
	Object* obj = AddObject(game, x, y, z, "res/enemy.mesh", "res/blue.tex", true);

	obj->tag = USERTAG_OBJECTIVE;
	collectibleLeft++;
}

static void AddCheckpoint(Game* game, int x, int y, int z)
{
	Object* obj = AddObject(game, x, y, z, "res/enemy.mesh", "res/blue.tex", true);

	obj->tag = USERTAG_CHECKPOINT;
}

static void AddTrap(Game* game, int x, int y, int z)
{
	Object* obj = AddObject(game, x, y, z, "res/plane.mesh", "res/trap.tex", true);

	obj->tag = USERTAG_TRAP;
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
	collectibleLeft = 0;
	_quantumEnergy = 0.f;

	{
		_player = OBJECT_New();
		_player->transform->position.z = -200.f; // TODO: Probably can do so we don't hardcode that uh
		_player->transform->position.y = CONFIG_GRID_UNIT_UP * 2.f;

		Renderer* r = RENDERER_New("res/enemy.mesh", "res/blue.tex");
		Collider* coll = COLLIDER_New(r);
		_playerRb = RIGIDBODY_New();

		coll->onCollision = OnPlayerCollision;

		OBJECT_AddComponent(_player, r->parent);
		OBJECT_AddComponent(_player, _playerRb->parent);
		OBJECT_AddComponent(_player, coll->parent);

		GAME_AddObject(g, _player);
	}
	AddPlatform(g, 0, 1, 0);

	AddPlatform(g, 1, 1, 0);
	AddObjective(g, 1, 2, 0);

	AddPlatform(g, -1, 1, 0);
	AddPlatform(g, -1, 1, 1);
	AddPlatform(g, -2, 1, 1);
	AddPlatform(g, -3, 1, 1);
	AddPlatform(g, -3, 1, 0);
	AddTrap(g, -2, 1, 0);
	AddObjective(g, -3, 1, 0);
	AddCheckpoint(g, -2, 1, 1);

	_initialPos = _player->transform->position;
	_camOffset = VECTOR_New(
		_cameraPos.x - _player->transform->position.x,
		_cameraPos.y - _player->transform->position.y,
		_cameraPos.z - _player->transform->position.z
	);
}