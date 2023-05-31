#include <math.h>

#include "component/renderer.h"
#include "component/rigidbody.h"
#include "component/collider.h"
#include "audio/audio.h"
#include "user.h"
#include "rendering/render.h"
#include "rendering/font.h"

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
static float _endTime;

static bool showCredits;

#ifdef _DEBUG
static int _lastFramerate;
static int _framerate;
static float _framerateTimer;
#endif

static bool _didWon;

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

static void OnPlayerCollision(Context* ctx, Game* game, Object* collision)
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
			_didWon = true;
			_playerRb->linearVelocity = VECTOR_Zero();
			_endTime = ctx->time->timeSinceStart;
		}
	}
	else if (collision->tag == USERTAG_CHECKPOINT)
	{
		_initialPos = VECTOR_New(collision->transform->position.x, _initialPos.y, collision->transform->position.z);
	}
	else if (collision->tag == USERTAG_CREDITS)
	{
		showCredits = true;
	}
}

void USER_Input(int key, bool isPressed)
{
	switch (key)
	{
	case 32: // Spacebar
		if (_playerRb->isOnGround)
		{
			RIGIDBODY_AddForce(_playerRb, VECTOR_New(.0f, CONFIG_JUMP_FORCE * (_quantumEnergy * .5f / 100.f + .5f), .0f));
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

void USER_Update(Game* g, Context* ctx)
{
	(void)g;

#ifdef _DEBUG
	_framerate++;
	_framerateTimer -= ctx->time->deltaTime;
	if (_framerateTimer <= 0.f)
	{
		_lastFramerate = _framerate;
		_framerate = 0;
		_framerateTimer = 1.f;
	}
	char fps[5];
	_itoa_s((int)_lastFramerate, fps, 5, 10);
	FONT_SetCursor(32, 64);
	FONT_Print(fps);
#endif

	if (showCredits)
	{
		FONT_SetCursor(150, 150);
		FONT_Print("3D Modelling\nJadith Nicole Kay Bruzenak\n\nMusic\nMidori\n\nProgramming\nTheIndra\nChristian Chaux\n\n\"Erika Type\"font by Peter Wiegel under OFL");
	}
	showCredits = false;
	if (_didWon)
	{
		char nb[4];
		FONT_SetCursor(32, 16);
		FONT_Print("You Won: ");
		_itoa_s((int)_endTime / 60, nb, 4, 10);
		FONT_Print(nb);
		FONT_Print(":");
		_itoa_s((int)_endTime % 60, nb, 4, 10);
		FONT_Print(nb);
		FONT_Print("min");
	}
	else // Move player
	{
		Vector dirVector = VECTOR_Multiply(VECTOR_Magnitude(VECTOR_New(
			(_isLeftPressed ? 1.f : 0.f) + (_isRightPressed ? -1.f : 0.f),
			.0f,
			(_isUpPressed ? -1.f : 0.f) + (_isDownPressed ? 1.f : 0.f)
		)), CONFIG_SPEED);
		_playerRb->linearVelocity.x = dirVector.x;
		_playerRb->linearVelocity.z = dirVector.z;

		if (dirVector.x != 0.f || dirVector.z != 0.f)
		{
			_player->transform->rotation = VECTOR_New(.0f, (float)atan2(dirVector.x, dirVector.z), .0f);
		}

		if (_playerRb->isOnGround)
		{
			_quantumEnergy += ctx->time->deltaTime * CONFIG_ENERGY_RELOAD_RATE;
			if (_quantumEnergy > 100.f) _quantumEnergy = 100.f;
		}

		{
			char nb[4];
			_itoa_s((int)_quantumEnergy, nb, 4, 10);

			FONT_SetCursor(32, 16);

			FONT_Print("Quantum Energy: ");
			FONT_Print(nb);
		}

		// Set camera position to follow player
		UpdateCameraPosition();
	}
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
	AddObject(game, x, y, z, "res/models/platform.mesh", "res/textures/colors.tex", false);
}

static void AddObjective(Game* game, int x, int y, int z, char* model)
{
	Object* obj = AddObject(game, x, y, z, model, "res/textures/colors.tex", true);

	Rigidbody* rb = RIGIDBODY_New();
	rb->useGravity = false;
	rb->angularVelocity = VECTOR_New(0.f, 3.f, 0.f);

	OBJECT_AddComponent(obj, rb->parent);

	obj->tag = USERTAG_OBJECTIVE;
	collectibleLeft++;
}

static void AddCheckpoint(Game* game, int x, int y, int z)
{
	Object* obj = AddObject(game, x, y, z, "res/models/checkpoint.mesh", "res/textures/colors.tex", true);

	Rigidbody* rb = RIGIDBODY_New();
	rb->useGravity = false;
	rb->angularVelocity = VECTOR_New(0.f, 3.f, 0.f);

	OBJECT_AddComponent(obj, rb->parent);

	obj->tag = USERTAG_CHECKPOINT;
}

static void AddTrap(Game* game, int x, int y, int z, float ox, float oz, bool high)
{
	Object* obj = AddObject(game, x, y, z, high ? "res/models/obstacles/cactus.mesh" : "res/models/obstacles/snake.mesh", "res/textures/colors.tex", true);
	obj->transform->position.x += ox;
	obj->transform->position.z += oz;
	obj->transform->position.y -= 20;

	obj->tag = USERTAG_TRAP;
}

static void AddObstacle(Game* game, int x, int y, int z, float ox, float oz)
{
	Object* obj = AddObject(game, x, y, z, "res/models/obstacles/rocks.mesh", "res/textures/colors.tex", false);
	obj->transform->position.x += ox;
	obj->transform->position.z += oz;
	obj->transform->position.y -= 20;
}

static void AddTombstone(Game* game, int x, int y, int z)
{
	Object* obj = AddObject(game, x, y, z, "res/models/obstacles/tombstone.mesh", "res/textures/colors.tex", true);
	Collider* triggerZone = OBJECT_GetComponent(obj, COMPONENT_COLLIDER);
	const int size = 25;
	triggerZone->min.x -= size;
	triggerZone->min.y -= size;
	triggerZone->min.z -= size;
	triggerZone->max.x += size;
	triggerZone->max.y += size;
	triggerZone->max.z += size;

	obj->tag = USERTAG_CREDITS;
}

static void CreateMap(Game* g)
{
	const int size = 10;
	const int halfSize = size / 2.f;

	const int P = 1;
	const int _ = 1;
	const int _2 = 6;
	const int _0 = 7;
	const int O = 2;
	const int C = 3;
	const int TL = 4;
	const int TH = 5;

	int floor[10][10] = {
		{ 0 , 0 , 0 , 0 , 0 , _0, 0 , 0 , 0 , O  },
		{ 0 , O , 0 , 0 , 0 , 0 , _ , _ , _ , 0  },
		{ _ , TL, C , _ , _ , _ , 0 , 0 , 0 , 0  },
		{ 0 , _ , 0 , 0 , 0 , _ , 0 , 0 , 0 , 0  },
		{ 0 , _ , 0 , 0 , 0 , _2, TL, _ , TL, O  },
		{ 0 , TL, O , TH, 0 , P , 0 , 0 , 0 , _  },
		{ 0 , _ , 0 , 0 , 0 , _ , 0 , 0 , _ , TL },
		{ 0 , _ , 0 , 0 , 0 , _ , 0 , 0 , TL, _  },
		{ 0 , C , _ , 0 , _ , _ , _ , 0 , C , TL },
		{ 0 , 0 , 0 , 0 , _ , _ , _ , 0 , 0 , 0  }
	};
	char collectibles[4][36] = {
		"res/models/collectibles/gun.mesh",
		"res/models/collectibles/bullet.mesh",
		"res/models/collectibles/lasso.mesh",
		"res/models/collectibles/spur.mesh"
	};
	int collectibleIndex = 0;

	for (int z = 0; z < size; z++)
	{
		for (int x = 0; x < size; x++)
		{
			if (floor[z][x])
			{
				int px = x - halfSize;
				int pz = z - halfSize;
				AddPlatform(g, px, 1, pz);
				switch (floor[z][x])
				{
				case 2: // Objective
					AddObjective(g, px, 2, pz, collectibles[collectibleIndex]);
					collectibleIndex++;
					break;

				case 3: // Checkpoint
					AddCheckpoint(g, px, 2, pz);
					break;

				case 4:
					AddTrap(g, px, 2, pz, 0, 0, false);
					AddTrap(g, px, 2, pz, 75, 75, false);
					AddTrap(g, px, 2, pz, 75, -75, false);
					AddTrap(g, px, 2, pz, -75, 75, false);
					AddTrap(g, px, 2, pz, -75, -75, false);
					break;

				case 5:
					AddTrap(g, px, 2, pz, 0, 0, true);
					AddTrap(g, px, 2, pz, 75, 75, true);
					AddTrap(g, px, 2, pz, 75, -75, true);
					AddTrap(g, px, 2, pz, -75, 75, true);
					AddTrap(g, px, 2, pz, -75, -75, true);
					break;

				case 6:
					AddObstacle(g, px, 2, pz, 0, 0);
					break;

				case 7:
					AddTombstone(g, px, 2, pz);
					break;
				}
			}
		}
	}
}

void USER_Init(Game* g, Context* ctx)
{
	(void)ctx;

	_cameraPos = VECTOR_New(0.f, 600.f, 200.f);
	_isLeftPressed = false;
	_isRightPressed = false;
	_isUpPressed = false;
	_isDownPressed = false;
	_canJump = true;
	collectibleLeft = 0;
	_quantumEnergy = 0.f;
#if _DEBUG
	_lastFramerate = 0;
	_framerate = 0;
	_framerateTimer = 1.f;
#endif
	_didWon = false;
	showCredits = false;

	{
		_player = OBJECT_New();
		_player->transform->position.z = -200.f; // TODO: Probably can do so we don't hardcode that uh
		_player->transform->position.y = CONFIG_GRID_UNIT_UP * 2.1f;

		Renderer* r = RENDERER_New("res/models/player.mesh", "res/textures/colors.tex");
		Collider* coll = COLLIDER_New(r);
		_playerRb = RIGIDBODY_New();

		coll->onCollision = OnPlayerCollision;

		OBJECT_AddComponent(_player, r->parent);
		OBJECT_AddComponent(_player, _playerRb->parent);
		OBJECT_AddComponent(_player, coll->parent);

		GAME_AddObject(g, _player);
	}

	CreateMap(g);

	_initialPos = _player->transform->position;
	_camOffset = VECTOR_New(
		_cameraPos.x - _player->transform->position.x,
		_cameraPos.y - _player->transform->position.y,
		_cameraPos.z - _player->transform->position.z
	);
}