#pragma once

#include "object.h"
#include "context/context.h"

struct Game {
	Object** objects;
};

Game* GAME_Init(void);
void GAME_Destroy(Game* g);
void GAME_Update(Game* g, Context* ctx);
void GAME_AddObject(Game* g, Object* obj);
void GAME_RemoveObject(Game* g, Object* obj);